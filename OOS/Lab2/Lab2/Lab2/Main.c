#include <windows.h>
#include <stdio.h>
#include <locale.h>

#define THREAD_COUNT 3

int bRun;

HANDLE hEvent;

DWORD WINAPI threadFunctionInc(LPVOID lpNumber)
{
    int* number = (int*)lpNumber;
    int threadId = GetCurrentThreadId();
    int b = 1;
    while (bRun)
    {
        WaitForSingleObject(hEvent, INFINITE);
        printf("Нить %d запущена\n", threadId);
        *number = *number + b;
        printf("a = a + %d = %d\n", b, *number);
        printf("Нить %d завершилась\n\n", threadId);
        SetEvent(hEvent);
    }
    return 0;
}

DWORD WINAPI threadFunctionDec(LPVOID lpNumber)
{
    int* number = (int*)lpNumber;
    int threadId = GetCurrentThreadId();
    int b = 2;
    while (bRun)
    {
        WaitForSingleObject(hEvent, INFINITE);
        printf("Нить %d запущена\n", threadId);
        *number = *number - b;
        printf("a = a - %d = %d\n", b, *number);
        printf("Нить %d завершилась\n\n", threadId);
        SetEvent(hEvent);
    }

    return 0;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    HANDLE hThreads[THREAD_COUNT];
    int number = 0;
    printf("a = %d\n\n", number);
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hEvent == NULL) {
        printf("Событие не создано (%d)\n", GetLastError());
        return 1;
    }
    for (int i = 0; i < THREAD_COUNT - 1; i++)
    {
        hThreads[i] = CreateThread(NULL, 0, threadFunctionInc, &number, NULL, NULL);
        if (hThreads[i] == NULL)
        {
            printf("Не удалось создать нить %d\n", i + 1);
            return 1;
        }
    }

    hThreads[THREAD_COUNT - 1] = CreateThread(NULL, 0, threadFunctionDec, &number, NULL, NULL);

    bRun = 1;
    SetEvent(hEvent);
    Sleep(5000);
    bRun = 0;
    WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);

    printf("a = %d\n\n", number);

    CloseHandle(hEvent);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        CloseHandle(hThreads[i]);
    }

    return 0;
}