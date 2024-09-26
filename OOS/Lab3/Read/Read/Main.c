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
    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (hMutex == NULL)
    {
        printf("�� ������� ������� ������� (%d).\n", GetLastError());
        return 1;
    }
    HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, FILE_MAPPING_NAME);
    if (hFileMapping == NULL)
    {
        printf("�� ������� ������� ����������� ����� (%d).\n", GetLastError());
        return 1;
    }
    LPVOID lpMapAddress = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, MAX_DATA_SIZE);
    if (lpMapAddress == NULL)
    {
        printf("�� ������� ���������� ���� (%d).\n", GetLastError());
        CloseHandle(hFileMapping);
        CloseHandle(hMutex);
        return 1;
    }

    WaitForSingleObject(hMutex, INFINITE);
    printf("\n��������� Read:\n");
    int result = system((char*)lpMapAddress);
    if (result == -1)
    {
        printf("������ �������\n");
    }
    ReleaseMutex(hMutex);

    UnmapViewOfFile(lpMapAddress);
    CloseHandle(hFileMapping);
    CloseHandle(hMutex);

    return 0;
}