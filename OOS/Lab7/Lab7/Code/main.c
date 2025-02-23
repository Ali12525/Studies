#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>

#define BUFFER_SIZE 4096 * 100

void swapBuffers(char** buffer1, char** buffer2) {
    char* temp = *buffer1;
    *buffer1 = *buffer2;
    *buffer2 = temp;
}

char transliterate�har(char c) {
    switch (c) {
    case '�': return 'a';
    case '�': return 'b';
    case '�': return 'v';
    case '�': return 'g';
    case '�': return 'd';
    case '�': return 'e';
    case '�': return 'e';
    case '�': return 'z';
    case '�': return 'z';
    case '�': return 'i';
    case '�': return 'y';
    case '�': return 'k';
    case '�': return 'l';
    case '�': return 'm';
    case '�': return 'n';
    case '�': return 'o';
    case '�': return 'p';
    case '�': return 'r';
    case '�': return 's';
    case '�': return 't';
    case '�': return 'u';
    case '�': return 'f';
    case '�': return 'h';
    case '�': return 'c';
    case '�': return 'c';
    case '�': return 's';
    case '�': return 's';
    case '�': return '\'';
    case '�': return 'y';
    case '�': return '\'';
    case '�': return 'e';
    case '�': return 'u';
    case '�': return 'y';
    case '�': return 'A';
    case '�': return 'B';
    case '�': return 'V';
    case '�': return 'G';
    case '�': return 'D';
    case '�': return 'E';
    case '�': return 'E';
    case '�': return 'Z';
    case '�': return 'Z';
    case '�': return 'I';
    case '�': return 'Y';
    case '�': return 'K';
    case '�': return 'L';
    case '�': return 'M';
    case '�': return 'N';
    case '�': return 'O';
    case '�': return 'P';
    case '�': return 'R';
    case '�': return 'S';
    case '�': return 'T';
    case '�': return 'U';
    case '�': return 'F';
    case '�': return 'H';
    case '�': return 'C';
    case '�': return 'C';
    case '�': return 'S';
    case '�': return 'S';
    case '�': return '\'';
    case '�': return 'Y';
    case '�': return '\'';
    case '�': return 'E';
    case '�': return 'U';
    case '�': return 'Y';

    default: return c;
    }
}

void transliterateBuffer(char* buffer, DWORD bytesRead) {
    for (DWORD i = 0; i < bytesRead; i++) {
        buffer[i] = transliterate�har(buffer[i]);
    }
}

int main() {
    HANDLE hSource, hDest;
    OVERLAPPED overlappedRead = { 0 }, overlappedWrite = { 0 };
    HANDLE hEventRead, hEventWrite;
    DWORD bytesRead = 0, bytesWritten = 0, bytesReadBefore = 0;
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    char* currentBuffer = buffer1;
    char* processingBuffer = buffer2;

    hEventRead = CreateEvent(NULL, TRUE, FALSE, NULL);
    hEventWrite = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (!hEventRead || !hEventWrite) {
        fprintf(stderr, "Failed to create events.\n");
        return 1;
    }

    hSource = CreateFileW(L"input.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hSource == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open input file.\n");
        return 1;
    }

    hDest = CreateFileW(L"output.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    if (hDest == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open output file.\n");
        CloseHandle(hSource);
        return 1;
    }

    overlappedRead.hEvent = hEventRead;
    overlappedWrite.hEvent = hEventWrite;

    for (int i = 0; 1; i++) {
        if (!ReadFile(hSource, currentBuffer, BUFFER_SIZE, &bytesRead, &overlappedRead)) {
            DWORD error = GetLastError();
            if (error != ERROR_IO_PENDING) {
                break;
            }
        }
        
        if (i == 0) {
            GetOverlappedResult(hSource, &overlappedRead, &bytesRead, TRUE);
            swapBuffers(&currentBuffer, &processingBuffer);
            overlappedRead.Offset += bytesRead;
            bytesReadBefore = bytesRead;

            continue;
        }

        transliterateBuffer(processingBuffer, bytesReadBefore);

        if (!WriteFile(hDest, processingBuffer, bytesReadBefore, &bytesWritten, &overlappedWrite)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                fprintf(stderr, "WriteFile failed.\n");
                break;
            }
        }

        GetOverlappedResult(hDest, &overlappedWrite, &bytesWritten, TRUE);
        if (!GetOverlappedResult(hSource, &overlappedRead, &bytesRead, TRUE)) {
            break;
        }

        bytesReadBefore = bytesRead;
        overlappedRead.Offset += bytesRead;
        overlappedWrite.Offset += bytesWritten;
        swapBuffers(&currentBuffer, &processingBuffer);
    }

    CloseHandle(hDest);
    hDest = CreateFileW(L"output.txt", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

    transliterateBuffer(processingBuffer, bytesReadBefore);

    if (!WriteFile(hDest, processingBuffer, bytesReadBefore, &bytesWritten, &overlappedWrite)) {
        if (GetLastError() != ERROR_IO_PENDING) {
            fprintf(stderr, "WriteFile failed.\n");
        }
    }
    GetOverlappedResult(hDest, &overlappedWrite, &bytesWritten, TRUE);

    CloseHandle(hSource);
    CloseHandle(hDest);
    CloseHandle(hEventRead);
    CloseHandle(hEventWrite);

    printf("Transliteration completed.\n");
    return 0;
}