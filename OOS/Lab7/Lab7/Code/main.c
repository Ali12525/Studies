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

char transliterateÑhar(char c) {
    switch (c) {
    case 'à': return 'a';
    case 'á': return 'b';
    case 'â': return 'v';
    case 'ã': return 'g';
    case 'ä': return 'd';
    case 'å': return 'e';
    case '¸': return 'e';
    case 'æ': return 'z';
    case 'ç': return 'z';
    case 'è': return 'i';
    case 'é': return 'y';
    case 'ê': return 'k';
    case 'ë': return 'l';
    case 'ì': return 'm';
    case 'í': return 'n';
    case 'î': return 'o';
    case 'ï': return 'p';
    case 'ğ': return 'r';
    case 'ñ': return 's';
    case 'ò': return 't';
    case 'ó': return 'u';
    case 'ô': return 'f';
    case 'õ': return 'h';
    case 'ö': return 'c';
    case '÷': return 'c';
    case 'ø': return 's';
    case 'ù': return 's';
    case 'ú': return '\'';
    case 'û': return 'y';
    case 'ü': return '\'';
    case 'ı': return 'e';
    case 'ş': return 'u';
    case 'ÿ': return 'y';
    case 'À': return 'A';
    case 'Á': return 'B';
    case 'Â': return 'V';
    case 'Ã': return 'G';
    case 'Ä': return 'D';
    case 'Å': return 'E';
    case '¨': return 'E';
    case 'Æ': return 'Z';
    case 'Ç': return 'Z';
    case 'È': return 'I';
    case 'É': return 'Y';
    case 'Ê': return 'K';
    case 'Ë': return 'L';
    case 'Ì': return 'M';
    case 'Í': return 'N';
    case 'Î': return 'O';
    case 'Ï': return 'P';
    case 'Ğ': return 'R';
    case 'Ñ': return 'S';
    case 'Ò': return 'T';
    case 'Ó': return 'U';
    case 'Ô': return 'F';
    case 'Õ': return 'H';
    case 'Ö': return 'C';
    case '×': return 'C';
    case 'Ø': return 'S';
    case 'Ù': return 'S';
    case 'Ú': return '\'';
    case 'Û': return 'Y';
    case 'Ü': return '\'';
    case 'İ': return 'E';
    case 'Ş': return 'U';
    case 'ß': return 'Y';

    default: return c;
    }
}

void transliterateBuffer(char* buffer, DWORD bytesRead) {
    for (DWORD i = 0; i < bytesRead; i++) {
        buffer[i] = transliterateÑhar(buffer[i]);
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