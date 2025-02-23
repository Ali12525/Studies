#include <windows.h>
#include <stdio.h>

HHOOK hHook = NULL;
typedef LRESULT(*ShellProc)(int, WPARAM, LPARAM);

int main() {
    HINSTANCE hLib = LoadLibrary(TEXT("HookWindAppLib.dll"));
    if (!hLib) {
        printf("Failed to load DLL.\n");
        return 1;
    }

    ShellProc shellProc = (ShellProc)GetProcAddress(hLib, "ShellProc");
    int* pStopFlag = (int*)GetProcAddress(hLib, "stopFlag");

    if (!shellProc || !pStopFlag) {
        printf("Failed to get function addresses.\n");
        FreeLibrary(hLib);
        return 1;
    }

    if (hHook == NULL) {
        hHook = SetWindowsHookEx(WH_SHELL, shellProc, hLib, 0);
        if (hHook == NULL) {
            wprintf(L"Failed to set hook\n");
            FreeLibrary(hLib);
            return 1;
        }
    }

    MSG msg;
    while (1) {
        if (*pStopFlag) {
            PostQuitMessage(0);
            break;
        }
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                break;
            }
        }
    }

    if (hHook != NULL) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
    FreeLibrary(hLib);
    return 0;
}
