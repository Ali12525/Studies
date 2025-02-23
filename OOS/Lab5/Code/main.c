#include <stdio.h>
#include <windows.h>
#include "serverLib.h"

typedef void (*StartClientFunc)();

void StartClient()
{
    HMODULE hClientLib = LoadLibrary(TEXT("ClientLib.dll"));
    if (!hClientLib) {
        printf("Failed to load client library. Error: %d\n", GetLastError());
    }
    StartClientFunc startClient = (StartClientFunc)GetProcAddress(hClientLib, "StartClient");
    if (!startClient) {
        printf("Failed to get StartClient function address. Error: %d\n", GetLastError());
        FreeLibrary(hClientLib);
    }

    startClient();
    FreeLibrary(hClientLib);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        return 1;
    }
    if (argv[1][0] == 'c') {
        StartClient();
    }
    else if (argv[1][0] == 's') {
        StartServer();
    }
    return 0;
}
