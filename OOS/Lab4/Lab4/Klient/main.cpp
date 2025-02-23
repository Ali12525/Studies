#include <stdio.h>
#include <windows.h>

#define CHECK_INTERVAL 2000

LPCTSTR MAILBOX_NAME = TEXT("\\\\.\\mailslot\\sample_mailslot");

typedef struct {
    OVERLAPPED ol;
    HANDLE hMailSlot;
    WCHAR buffer[256];
} MAILSLOT_OVERLAPPED;

VOID CALLBACK WriteCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    if (dwErrorCode == ERROR_SUCCESS) {
        wprintf(L"Data written successfully: %d bytes\n\n", dwNumberOfBytesTransfered);
    }
    else {
        wprintf(L"Failed to write file. Error: %d\n", dwErrorCode);
    }
}

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue) {
    WCHAR windowsDir[MAX_PATH];
    WCHAR tempPath[MAX_PATH];

    UINT winLength = GetWindowsDirectory(windowsDir, MAX_PATH);
    if (winLength == 0) {
        wprintf(L"Failed to get Windows directory. Error: %d\n", GetLastError());
    }
    else {
        wprintf(L"Windows Directory: %s\n", windowsDir);
    }

    DWORD tempLength = GetTempPath(MAX_PATH, tempPath);
    if (tempLength == 0) {
        wprintf(L"Failed to get temp path. Error: %d\n", GetLastError());
    }
    else {
        wprintf(L"Temp Path: %s\n", tempPath);
    }

    MAILSLOT_OVERLAPPED* mailslot_ol = (MAILSLOT_OVERLAPPED*)lpArg;
    WCHAR* message = mailslot_ol->buffer;
    swprintf(message, sizeof(mailslot_ol->buffer) / sizeof(WCHAR), L"Windows Directory: %s\nTemp Path: %s\n\n", windowsDir, tempPath);
    BOOL result = WriteFileEx(mailslot_ol->hMailSlot, message, (DWORD)(wcslen(message) * sizeof(WCHAR)), &mailslot_ol->ol, WriteCompletionRoutine);
    if (!result) {
        wprintf(L"WriteFileEx failed. Error: %d\n", GetLastError());
    }
}

int main(int argc, char* argv[]) {
    wprintf(L"CLIENT\n");

    HANDLE hMailSlot = CreateFile(MAILBOX_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hMailSlot == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to open mailslot. Error: %d\n", GetLastError());
        return 1;
    }

    MAILSLOT_OVERLAPPED mailslot_ol;
    SecureZeroMemory(&mailslot_ol, sizeof(mailslot_ol));
    mailslot_ol.hMailSlot = hMailSlot;

    HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (!hTimer) {
        wprintf(L"CreateWaitableTimer failed. Error: %d\n", GetLastError());
        CloseHandle(hMailSlot);
        return 1;
    }

    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -100000LL;
    if (!SetWaitableTimer(hTimer, &liDueTime, CHECK_INTERVAL, TimerAPCProc, &mailslot_ol, FALSE)) {
        wprintf(L"SetWaitableTimer failed. Error: %d\n", GetLastError());
        CloseHandle(hMailSlot);
        CloseHandle(hTimer);
        return 1;
    }

    while (TRUE) {
        SleepEx(INFINITE, TRUE);
    }

    CloseHandle(hMailSlot);
    CloseHandle(hTimer);
    return 0;
}