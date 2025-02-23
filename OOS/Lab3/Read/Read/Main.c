#include <locale.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_DATA_SIZE 256
#define MUTEX_NAME "WriteRead"
#define FILE_MAPPING_NAME "FileMapping"

int main()
{
    setlocale(LC_ALL, "Rus");
    HANDLE hMutex1 = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (hMutex1 == NULL)
    {
        printf("Не удалось открыть мьютекс (%d).\n", GetLastError());
        return 1;
    }
    HANDLE hMutex2 = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (hMutex2 == NULL)
    {
        printf("Не удалось открыть мьютекс (%d).\n", GetLastError());
        return 1;
    }
    HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, FILE_MAPPING_NAME);
    if (hFileMapping == NULL)
    {
        printf("Не удалось открыть отображение файла (%d).\n", GetLastError());
        return 1;
    }
    LPVOID lpMapAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAX_DATA_SIZE);
    if (lpMapAddress == NULL)
    {
        printf("Не удалось отобразить файл (%d).\n", GetLastError());
        CloseHandle(hFileMapping);
        CloseHandle(hMutex1);
        CloseHandle(hMutex2);
        return 1;
    }

    while (1)
    {
        WaitForSingleObject(hMutex1, INFINITE);
        printf("\nПрограмма Read:\n");
        int result = system((char*)lpMapAddress);
        if (result == -1)
        {
            printf("Ошибка команды\n");
        }
        ReleaseMutex(hMutex2);
    }

    UnmapViewOfFile(lpMapAddress);
    CloseHandle(hFileMapping);
    CloseHandle(hMutex1);
    CloseHandle(hMutex2);

    return 0;
}