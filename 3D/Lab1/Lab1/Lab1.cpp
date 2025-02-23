// Lab1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab1.h"
#define _USE_MATH_DEFINES
#include <cmath>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

double radAngle = 45 * M_PI / 180;

double m = 0;
double n = 0;

double rotateMatrix[3][3] = {
            {cos(radAngle), sin(radAngle), 1},
           {-sin(radAngle), cos(radAngle), 1 },
           { 0, 0, 1},
};
double mirrorMatrix[3][3] = {
    {1, 0, 0},
    {0, -1, 0 },
    { 0, 460, 1},
};
double scaleMatrix[3][3] = {
    {1, 0, 0},
    {0, 0.5, 0 },
    { 0, 0, 1},
};
double massive[18][3] = {
           { 440, 320, 1},
           { 400, 280, 1},
           { 440, 240, 1},
           { 480, 280, 1},
           { 440, 320, 1},
           { 440, 380, 1},
           { 500, 415, 1},
           { 540, 415, 1},
           { 500, 415, 1},
           { 440, 380, 1},
           { 380, 415, 1},
           { 340, 415, 1},
           { 380, 415, 1},
           { 440, 380, 1},
           { 440, 335, 1},
           { 540, 360, 1},
           { 440, 335, 1},
           { 340, 360, 1}
};
double massiveCopy[18][3] = {
           { 440, 320, 1},
           { 400, 280, 1},
           { 440, 240, 1},
           { 480, 280, 1},
           { 440, 320, 1},
           { 440, 380, 1},
           { 500, 415, 1},
           { 540, 415, 1},
           { 500, 415, 1},
           { 440, 380, 1},
           { 380, 415, 1},
           { 340, 415, 1},
           { 380, 415, 1},
           { 440, 380, 1},
           { 440, 335, 1},
           { 540, 360, 1},
           { 440, 335, 1},
           { 340, 360, 1}
};

void massiveToOrigin()
{
    for (int i = 0; i < 18; i++)
    {
        massive[i][0] = massiveCopy[i][0];
        massive[i][1] = massiveCopy[i][1];
        massive[i][2] = massiveCopy[i][1];
    }
}

void MatrixMult(double matB[3][3])
{
    //double** nMat = new double* [m];
    double tnpX = 0;
    double tnpY;


    for (int i = 0; i < 18; i++)
    {
        tnpX = 0;
        tnpY = 0;
        for (int j = 0; j < 3; j++)
        {
            tnpX = tnpX + massive[i][j] * matB[j][0];
            tnpY = tnpY + massive[i][j] * matB[j][1];
        }
        massive[i][0] = tnpX;
        massive[i][1] = tnpY;
    }

}

void getMAndN()
{
    double minX = 10000;
    double maxX = -10000;
    double minY = 10000;
    double maxY = -10000;

    

    for (int i = 0; i < 18; i++)
    {
        if (massive[i][0] > maxX)
            maxX = massive[i][0];
        if (massive[i][1] > maxY)
            maxY = massive[i][1];
        if (massive[i][0] < minX)
            minX = massive[i][0];
        if (massive[i][1] < minY)
            minY = massive[i][1];
    }

    m = (maxX + minX) / 2;
    n = (maxY + minY) / 2;

}


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
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
            case IDM_ROTATE:
                getMAndN();
                rotateMatrix[2][0] = -m * (cos(radAngle) - 1) + n * sin(radAngle);
                rotateMatrix[2][1] = -n * (cos(radAngle) - 1) - m * sin(radAngle);
                MatrixMult(rotateMatrix);
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                break;
            case IDM_MIRROR:

                MatrixMult(mirrorMatrix);
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                break;
            case IDM_SCALE:

                MatrixMult(scaleMatrix);
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                break;
            case IDM_ORIGIN:
                massiveToOrigin();
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
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

            double x;
            double y;
            MoveToEx(hdc, massive[0][0], massive[0][1], 0);
            for (int i = 1; i < 18; i++) {
                x = massive[i][0];
                y = massive[i][1];
                LineTo(hdc, x, y);
            }

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
