// Lab42.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab42.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int r, g, b, SIZE_X = 200, SIZE_Y = 200;
int arr[10][3][3] =
{
    {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    },
    {
        {0,0,0},
        {0,0,0},
        {1,0,0}
    },
    {
        {0,0,0},
        {0,0,0},
        {1,1,0}
    },
    {
        {0,0,0},
        {0,0,0},
        {1,1,1}
    },
    {
        {0,0,0},
        {1,0,0},
        {1,1,1}
    },
    {
        {0,0,0},
        {1,1,0},
        {1,1,1}
    },
    {
        {0,0,0},
        {1,1,1},
        {1,1,1}
    },
    {
        {1,0,0},
        {1,1,1},
        {1,1,1}
    },
    {
        {1,1,0},
        {1,1,1},
        {1,1,1}
    },
    {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    }
};

int arr2[10][3][3] =
{
    {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    },
    {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    },
    {
        {0,0,0},
        {0,1,0},
        {1,0,0}
    },
    {
        {1,0,0},
        {0,1,0},
        {1,0,0}
    },
    {
        {1,0,1},
        {0,1,0},
        {1,0,0}
    },
    {
        {1,0,1},
        {0,1,0},
        {1,0,1}
    },
    {
        {1,0,1},
        {0,1,0},
        {1,1,1}
    },
    {
        {1,0,1},
        {1,1,0},
        {1,1,1}
    },
    {
        {1,1,1},
        {1,1,0},
        {1,1,1}
    },
    {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    }
};

int arrCount1X[200] = { 0 };
int arrCount1Y[200] = { 0 };
int arrCount2X[600] = { 0 };
int arrCount2Y[600] = { 0 };
int arrCount3X[600] = { 0 };
int arrCount3Y[600] = { 0 };

void setPixel(HDC hdc, int (*arrey)[3][3], int offsetX, int offsetY, int x, int y, int number, int average)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (arrey[number][i][j] == 0)
                SetPixel(hdc, offsetX + 3 * x + i, offsetY + 3 * y + j, RGB(0, 0, 0));
            else
                SetPixel(hdc, offsetX + 3 * x + i, offsetY + 3 * y + j, RGB(average, average, average));
        }
    }
}

void printHistogramX(HDC hdc, int* arrey, int offsetX, int offsetY, int sizeX, int sizeY)
{
    int max = 1;

    for (size_t x = 0; x < sizeX; x++)
    {
        for (size_t y = 0; y < sizeY; y++)
        {
            COLORREF col = GetPixel(hdc, x + offsetX, y + offsetY);
            r = GetRValue(col);
            g = GetGValue(col);
            b = GetBValue(col);
            if (r == 0 && g == 0 && b == 0)
                arrey[x]++;
        }
        if (arrey[x] > max)
            max = arrey[x];
    }

    for (size_t i = 0; i < sizeX; i++)
    {
        arrey[i] = arrey[i] * 150 / max;
    }

    for (size_t x = 0; x < sizeX; x++)
    {
        for (size_t y = 0; y < arrey[x]; y++)
        {
            SetPixel(hdc, x + offsetX, y + offsetY + sizeY + 20, RGB(0, 0, 0));
        }
    }
}

void printHistogramY(HDC hdc, int* arrey, int offsetX, int offsetY, int sizeX, int sizeY)
{
    int max = 1;

    for (size_t y = 0; y < sizeY; y++)
    {
        for (size_t x = 0; x < sizeX; x++)
        {
            COLORREF col = GetPixel(hdc, x + offsetX, y + offsetY);
            r = GetRValue(col);
            g = GetGValue(col);
            b = GetBValue(col);
            if (r == 0 && g == 0 && b == 0)
                arrey[y]++;
        }
        if (arrey[y] > max)
            max = arrey[y];
    }

    for (size_t i = 0; i < sizeY; i++)
    {
        arrey[i] = arrey[i] * 150 / max;
    }

    for (size_t y = 0; y < sizeY; y++)
    {
        for (size_t x = 0; x < arrey[y]; x++)
        {
            SetPixel(hdc, x + offsetX + sizeX + 20, y + offsetY, RGB(0, 0, 0));
        }
    }
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
    LoadStringW(hInstance, IDC_LAB42, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB42));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB42));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB42);
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
            HBITMAP img;
            HDC memdc;
            img = (HBITMAP)LoadImage(NULL, L"pic.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            memdc = CreateCompatibleDC(hdc);
            SelectObject(memdc, img);
            BitBlt(hdc, 0, 0, SIZE_X, SIZE_Y, memdc, 0, 0, SRCCOPY);
            COLORREF col;

            for (size_t x = 0; x < SIZE_X; x++)
            {
                for (size_t y = 0; y < SIZE_Y; y++)
                {
                    col = GetPixel(hdc, x, y);
                    r = GetRValue(col);
                    g = GetGValue(col);
                    b = GetBValue(col);

                    int average = (r + g + b) / 3;
                    int number = average / 25.5;
                    setPixel(hdc, arr, 0, 210, x, y, number, average);
                    setPixel(hdc, arr2, 780, 0, x, y, number, average);
                }
            }

            printHistogramX(hdc, arrCount2X, 0, 210, 600, 600);
            printHistogramY(hdc, arrCount2Y, 0, 210, 600, 600);
            printHistogramX(hdc, arrCount3X, 780, 0, 600, 600);
            printHistogramY(hdc, arrCount3Y, 780, 0, 600, 600);

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
