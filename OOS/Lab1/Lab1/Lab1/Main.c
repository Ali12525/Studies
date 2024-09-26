#include <windows.h>
#include <stdio.h>
#include <locale.h>

#define THREAD_COUNT 3

DWORD WINAPI threadFunction()
{
    Sleep(5000);

    return 0;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    HANDLE hThreads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        hThreads[i] = CreateThread(NULL, 0, threadFunction, NULL, CREATE_SUSPENDED, NULL);
        if (hThreads[i] == NULL)
        {
            printf("Не удалось создать нить %d\n", i + 1);
            return 1;
        }
    }

    do 
    {
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            ResumeThread(hThreads[i]);
            printf("Нить номер %d начала / возобновила работу\n", i + 1);
            Sleep(2000);
            SuspendThread(hThreads[i]);
            printf("Нить номер %d приостановлена\n\n", i + 1);
        }
    } while (WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, 50));

    for (int i = 0; i < THREAD_COUNT; i++)
        CloseHandle(hThreads[i]);

    printf("Все нити завершены.\n");

    return 0;
}