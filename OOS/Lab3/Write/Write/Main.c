#include <locale.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_DATA_SIZE 256
#define MUTEX_NAME "WriteRead"
#define FILE_MAPPING_NAME "FileMapping"

void startReadProgram()
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    WCHAR name[] = L"E:\\OOS\\Lab3\\Read\\x64\\Debug\\Read.exe";
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    if (!CreateProcess(NULL, name, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}   

int main() {
    setlocale(LC_ALL, "Rus");
    char command[100];
    HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);
    if (hMutex == NULL)
    {
        printf("Не удалось создать мьютекс (%d).\n", GetLastError());
        return 1;
    }
    HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_DATA_SIZE, FILE_MAPPING_NAME);
    if (hFileMapping == NULL)
    {
        printf("Не удалось создать отображение файла (%d).\n", GetLastError());
        return 1;
    }
    LPVOID lpMapAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAX_DATA_SIZE);
    if (lpMapAddress == NULL)
    {
        printf("Не удалось отобразить файл (%d).\n", GetLastError());
        CloseHandle(hFileMapping);
        CloseHandle(hMutex);
        return 1;
    }
    startReadProgram();
    printf("Программа Write:\n");
    printf("Введите команду: ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';
    sprintf_s((char*)lpMapAddress, MAX_DATA_SIZE, command);
    ReleaseMutex(hMutex);

    UnmapViewOfFile(lpMapAddress);
    CloseHandle(hFileMapping);  
    CloseHandle(hMutex);

    return 0;
}