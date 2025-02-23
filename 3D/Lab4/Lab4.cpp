// Lab4.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab4.h"

#define MAX_LOADSTRING 100
const int WIN_SIZE_X = 200;
const int WIN_SIZE_Y = 200;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int arrFrequency[256] = { 0 };
int arrFrequency2[256] = { 0 };
int arrFrequency3[256] = { 0 };
int arrFrequency4[256] = { 0 };
int arrFrequency5[256] = { 0 };
COLORREF col[3][3];

void getCol(HDC hdc, int x, int y, int offsetX, int offsetY)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            col[i][j] = GetPixel(hdc, x - 1 + j + offsetX, y - 1 + i + offsetY);
        }
    }
}

void filArr(HDC hdc, int x, int y, int offsetX, int offsetY, int* arrey)
{
    COLORREF newCol = GetPixel(hdc, x + offsetX, y + offsetY);
    int newRed = GetRValue(newCol);
    int newGreen = GetGValue(newCol);
    int newBlue = GetBValue(newCol);
    int brightness = (0.2126 * newRed + 0.7152 * newGreen + 0.0722 * newBlue);
    arrey[brightness]++;
}

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void printHistogram(HDC hdc, int* arrFrequency, int offsetX, int offsetY)
{
    int max = 1;

    for (size_t i = 0; i < 256; i++)
    {
        if (arrFrequency[i] >= max) max = arrFrequency[i];
    }

    for (size_t i = 0; i < 256; i++)
    {
        arrFrequency[i] = arrFrequency[i] * 200 / max;
    }

    for (size_t x = 0; x < 256; x++)
    {
        for (size_t y = 200; arrFrequency[x] != 0; arrFrequency[x]--, y--)
        {
            SetPixel(hdc, x + offsetX, y + offsetY, RGB(0, 0, 0));
        }
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            HBITMAP origPic = (HBITMAP)LoadImage(NULL, L"pic.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            HDC memdc = CreateCompatibleDC(hdc);
            SelectObject(memdc, origPic);
            BitBlt(hdc, 0, 0, 200, 200, memdc, 0, 0, SRCCOPY);
            BitBlt(hdc, 600, 0, 200, 200, memdc, 0, 0, SRCCOPY);
            BitBlt(hdc, 600, 250, 200, 200, memdc, 0, 0, SRCCOPY);
            int k[3][3] = { {1,1,1},
                            {1,2,1},
                            {1,1,1} };
            int k2[3][3] = { {1,2,1},
                            {2,4,2},
                            {1,2,1} };
            int red[3][3] = { {0,0,0},
                            {0,0,0},
                            {0,0,0} };
            int green[3][3] = { {0,0,0},
                            {0,0,0},
                            {0,0,0} };
            int blue[3][3] = { {0,0,0},
                            {0,0,0},
                            {0,0,0} };
            int newRed, newGreen, newBlue;
            COLORREF newCol;

            int sumK = 0;
            int sumK2 = 0;

            for (size_t i = 0; i < 3; i++)
            {
                for (size_t j = 0; j < 3; j++)
                {
                    sumK += k[i][j];
                    sumK2 += k2[i][j];
                }
            }

            for (size_t x = 1; x < WIN_SIZE_X - 1; x++)
            {
                for (size_t y = 1; y < WIN_SIZE_Y - 1; y++)
                {
                    int sumRedK = 0, sumGreenK = 0, sumBlueK = 0;
                    int sumRedK2 = 0, sumGreenK2 = 0, sumBlueK2 = 0;
                    getCol(hdc, x, y, 0, 0);

                    for (size_t i = 0; i < 3; i++)
                    {
                        for (size_t j = 0; j < 3; j++)
                        {
                            red[i][j] = GetRValue(col[i][j]);
                            green[i][j] = GetGValue(col[i][j]);
                            blue[i][j] = GetBValue(col[i][j]);

                            sumRedK += k[i][j] * red[i][j];
                            sumGreenK += k[i][j] * green[i][j];
                            sumBlueK += k[i][j] * blue[i][j];

                            sumRedK2 += k2[i][j] * red[i][j];
                            sumGreenK2 += k2[i][j] * green[i][j];
                            sumBlueK2 += k2[i][j] * blue[i][j];
                        }
                    }
                    
                    newRed = sumRedK / sumK;
                    newGreen = sumGreenK / sumK;
                    newBlue = sumBlueK / sumK;

                    newCol = RGB(newRed, newGreen, newBlue);
                    SetPixel(hdc, x, y + 250, newCol);

                    newRed = sumRedK2 / sumK2;
                    newGreen = sumGreenK2 / sumK2;
                    newBlue = sumBlueK2 / sumK2;

                    newCol = RGB(newRed, newGreen, newBlue);
                    SetPixel(hdc, x, y + 500, newCol);

                    sumRedK = 0, sumGreenK = 0; sumBlueK = 0, sumRedK2 = 0, sumGreenK2 = 0, sumBlueK2 = 0;
                    getCol(hdc, x, y, 600, 0);

                    for (size_t i = 0; i < 3; i++)
                    {
                        for (size_t j = 0; j < 3; j++)
                        {
                            red[i][j] = GetRValue(col[i][j]);
                            green[i][j] = GetGValue(col[i][j]);
                            blue[i][j] = GetBValue(col[i][j]);

                            sumRedK += k[i][j] * red[i][j];
                            sumGreenK += k[i][j] * green[i][j];
                            sumBlueK += k[i][j] * blue[i][j];
                        }
                    }

                    newRed = sumRedK / sumK;
                    newGreen = sumGreenK / sumK;
                    newBlue = sumBlueK / sumK;

                    newCol = RGB(newRed, newGreen, newBlue);
                    SetPixel(hdc, x + 600, y, newCol);

                    getCol(hdc, x, y, 600, 250);

                    for (size_t i = 0; i < 3; i++)
                    {
                        for (size_t j = 0; j < 3; j++)
                        {
                            red[i][j] = GetRValue(col[i][j]);
                            green[i][j] = GetGValue(col[i][j]);
                            blue[i][j] = GetBValue(col[i][j]);

                            sumRedK2 += k2[i][j] * red[i][j];
                            sumGreenK2 += k2[i][j] * green[i][j];
                            sumBlueK2 += k2[i][j] * blue[i][j];
                        }
                    }

                    newRed = sumRedK2 / sumK2;
                    newGreen = sumGreenK2 / sumK2;
                    newBlue = sumBlueK2 / sumK2;

                    newCol = RGB(newRed, newGreen, newBlue);
                    SetPixel(hdc, x + 600, y + 250, newCol);

                    filArr(hdc, x, y, 0, 0, arrFrequency);
                    filArr(hdc, x, y, 0, 250, arrFrequency2);
                    filArr(hdc, x, y, 0, 500, arrFrequency3);
                    filArr(hdc, x, y, 600, 0, arrFrequency4);
                    filArr(hdc, x, y, 600, 250, arrFrequency5);
                }
            }

            printHistogram(hdc, arrFrequency, 220, 0);
            printHistogram(hdc, arrFrequency2, 220, 250);
            printHistogram(hdc, arrFrequency3, 220, 500);
            printHistogram(hdc, arrFrequency4, 640, 0);
            printHistogram(hdc, arrFrequency5, 640, 250);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
