#include <windows.h>
#include <stdio.h>
#include "hookWindAppLib.h"

int stopFlag = 0;

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HSHELL_WINDOWCREATED && stopFlag == 0) {
        stopFlag = 1;
        MessageBox(NULL, L"New window has been created!", L"Window Creation Alert", MB_OK);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}