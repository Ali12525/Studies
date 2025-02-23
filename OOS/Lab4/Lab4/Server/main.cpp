#include <stdio.h>
#include <windows.h>

#define CHECK_INTERVAL 500

LPCTSTR MAILBOX_NAME = TEXT("\\\\.\\mailslot\\sample_mailslot");

typedef struct {
    OVERLAPPED ol;
    HANDLE hMailSlot;
    WCHAR buffer[256];
} MAILSLOT_OVERLAPPED;

VOID CALLBACK ReadCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    MAILSLOT_OVERLAPPED* mailslot_ol = (MAILSLOT_OVERLAPPED*)lpOverlapped;
    WCHAR* buffer = mailslot_ol->buffer;

    buffer[dwNumberOfBytesTransfered / sizeof(WCHAR)] = L'\0';
    wprintf(L"Received message: \n%s", buffer);
}

VOID CALLBACK TimerAPCProc(LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue) {
    MAILSLOT_OVERLAPPED* mailslot_ol = (MAILSLOT_OVERLAPPED*)lpArg;

    DWORD dwMaxMessageSize, dwNextSize, dwMessageCount, dwReadTimeout;
    if (!GetMailslotInfo(mailslot_ol->hMailSlot, &dwMaxMessageSize, &dwNextSize, &dwMessageCount, &dwReadTimeout)) {
        wprintf(L"GetMailslotInfo failed. Error: %d\n", GetLastError());
        return;
    }

    if (dwNextSize != MAILSLOT_NO_MESSAGE) {
        if (!ReadFileEx(mailslot_ol->hMailSlot, mailslot_ol->buffer, sizeof(mailslot_ol->buffer), &mailslot_ol->ol, ReadCompletionRoutine)) {
            wprintf(L"ReadFileEx failed. Error: %d\n", GetLastError());
        }
    }
}

int main(int argc, char* argv[]) {
    wprintf(L"SERVER\n");

    HANDLE hMailSlot = CreateMailslot(MAILBOX_NAME, 0, MAILSLOT_WAIT_FOREVER, NULL);
    if (hMailSlot == INVALID_HANDLE_VALUE) {
        wprintf(L"Failed to create mailslot. Error: %d\n", GetLastError());
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
    liDueTime.QuadPart = -10000000LL;
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