#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#pragma comment(lib, "Comdlg32.lib")

#include "IDMenu.h"
#include "AppState.h"
#include "Command.h"
#include "DrawShapeCommand.h"
#include "SetColorCommand.h"
#include "SetPenWidthCommand.h"
#include "MacroCommand.h"
#include "History.h"
#include "DrawingUtils.h"

static AppState g_state;
static History g_history;
static bool g_recording = false;
static std::vector<std::unique_ptr<ICommand>> g_macro_recorded;

static int g_selectedShape = 0;
static int g_pendingShape = 0;    // текущий размещаемый тип
static bool g_isDragging = false;
static POINT g_dragStart = { 0,0 };
static RECT g_currentRect = { 0,0,0,0 };
static const int MIN_SIZE = 10;

// Открывает стандартный диалог выбора цвета и возвращает выбранный цвет.
// Если пользователь отменяет, возвращает исходный цвет.
COLORREF PickColor(HWND hwnd, COLORREF initial) {
    CHOOSECOLOR cc = { sizeof(cc) };
    static COLORREF cust[16] = { 0 };
    cc.hwndOwner = hwnd;
    cc.lpCustColors = cust;
    cc.rgbResult = initial;
    cc.lStructSize = sizeof(cc);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    if (ChooseColor(&cc)) return cc.rgbResult;
    return initial;
}

void UpdateWindowTitlePending(HWND hwnd) {
    const wchar_t* base = L"Паттерн Команда";
    wchar_t buf[256];
    if (g_pendingShape != 0) {
        const wchar_t* sname = L"";
        if (g_pendingShape == 1) sname = L"Треугольник";
        else if (g_pendingShape == 2) sname = L"Прямоугольник";
        else if (g_pendingShape == 3) sname = L"Эллипс";
        swprintf_s(buf, L"%s — %s: drag to set size (Esc to cancel)", base, sname);
    }
    else if (g_selectedShape != 0) {
        const wchar_t* sname = L"";
        if (g_selectedShape == 1) sname = L"Треугольник";
        else if (g_selectedShape == 2) sname = L"Прямоугольник";
        else if (g_selectedShape == 3) sname = L"Эллипс";
        swprintf_s(buf, L"%s — Selected: %s (choose again to start placing)", base, sname);
    }
    else {
        swprintf_s(buf, L"%s", base);
    }
    SetWindowText(hwnd, buf);
}

void UpdateMenu(HWND hwnd) {
    HMENU hMenu = GetMenu(hwnd);
    EnableMenuItem(hMenu, IDM_UNDO, MF_BYCOMMAND | (g_history.CanUndo() ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_REDO, MF_BYCOMMAND | (g_history.CanRedo() ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_MACRO_STOP, MF_BYCOMMAND | (g_recording ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_MACRO_START, MF_BYCOMMAND | (g_recording ? MF_GRAYED : MF_ENABLED));
    EnableMenuItem(hMenu, IDM_MACRO_PLAY, MF_BYCOMMAND | (!g_macro_recorded.empty() ? MF_ENABLED : MF_GRAYED));
    DrawMenuBar(hwnd);
}

HMENU CreateAppMenu() {
    HMENU hMenu = CreateMenu();
    HMENU menuShapes = CreateMenu();
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_TRIANGLE, L"Треугольник");
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_RECT, L"Прямоугольник");
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_ELLIPSE, L"Эллипс");

    HMENU menuParams = CreateMenu();
    AppendMenu(menuParams, MF_STRING, IDM_COLOR_PICK, L"Выбрать цвет...");
    AppendMenu(menuParams, MF_SEPARATOR, 0, NULL);
    AppendMenu(menuParams, MF_STRING, IDM_PEN_1, L"Толщина 1");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_2, L"Толщина 2");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_3, L"Толщина 3");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_5, L"Толщина 5");

    HMENU menuEdit = CreateMenu();
    AppendMenu(menuEdit, MF_STRING, IDM_UNDO, L"Отмена");
    AppendMenu(menuEdit, MF_STRING, IDM_REDO, L"Повтор");

    HMENU menuMacro = CreateMenu();
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_START, L"Начать запись");
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_STOP, L"Остановить запись");
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_PLAY, L"Выполнить макрокоманду");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuShapes, L"Фигуры");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuParams, L"Параметры");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuEdit, L"Правка");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuMacro, L"Макро");

    return hMenu;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        SetMenu(hwnd, CreateAppMenu());
        UpdateMenu(hwnd);
        UpdateWindowTitlePending(hwnd);
        break;
    }

    case WM_KEYDOWN: {
        if (wParam == VK_ESCAPE) {
            if (g_isDragging) {
                g_isDragging = false;
                ReleaseCapture();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (g_pendingShape != 0) {
                g_pendingShape = 0;
                UpdateWindowTitlePending(hwnd);
            }
        }
        break;
    }

    case WM_LBUTTONDOWN: {
        if (g_pendingShape != 0 && !g_isDragging) {
            g_isDragging = true;
            g_dragStart.x = LOWORD(lParam);
            g_dragStart.y = HIWORD(lParam);
            g_currentRect.left = g_dragStart.x;
            g_currentRect.top = g_dragStart.y;
            g_currentRect.right = g_dragStart.x;
            g_currentRect.bottom = g_dragStart.y;
            SetCapture(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }

    case WM_MOUSEMOVE: {
        if (g_isDragging) {
            POINT pt; pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
            g_currentRect.left = g_dragStart.x;
            g_currentRect.top = g_dragStart.y;
            g_currentRect.right = pt.x;
            g_currentRect.bottom = pt.y;
            g_currentRect = NormalizeRect(g_currentRect);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }

    case WM_LBUTTONUP: {
        if (g_isDragging) {
            POINT pt; pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
            RECT finalRect = { g_dragStart.x, g_dragStart.y, pt.x, pt.y };
            finalRect = NormalizeRect(finalRect);

            if ((finalRect.right - finalRect.left) < MIN_SIZE)
                finalRect.right = finalRect.left + MIN_SIZE;
            if ((finalRect.bottom - finalRect.top) < MIN_SIZE)
                finalRect.bottom = finalRect.top + MIN_SIZE;

            ShapeType t = ShapeType::Rectangle;
            if (g_pendingShape == 1) t = ShapeType::Triangle;
            else if (g_pendingShape == 2) t = ShapeType::Rectangle;
            else if (g_pendingShape == 3) t = ShapeType::Ellipse;

            auto cmd = std::make_unique<DrawShapeCommand>(t, finalRect, g_state.currentColor, g_state.currentPenWidth);
            g_history.ExecuteAndPush(std::move(cmd), g_state, hwnd, g_recording, &g_macro_recorded);

            g_isDragging = false;
            ReleaseCapture();
            if (g_selectedShape != 0) g_pendingShape = g_selectedShape;
            else g_pendingShape = 0;

            UpdateMenu(hwnd);
            UpdateWindowTitlePending(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDM_SHAPE_TRIANGLE:
        case IDM_SHAPE_RECT:
        case IDM_SHAPE_ELLIPSE: {
            int chosen = 0;
            if (id == IDM_SHAPE_TRIANGLE) chosen = 1;
            else if (id == IDM_SHAPE_RECT) chosen = 2;
            else if (id == IDM_SHAPE_ELLIPSE) chosen = 3;
            g_selectedShape = chosen;
            g_pendingShape = chosen;
            UpdateWindowTitlePending(hwnd);
            break;
        }
        case IDM_COLOR_PICK: {
            COLORREF picked = PickColor(hwnd, g_state.currentColor);
            if (picked != g_state.currentColor) {
                auto cmd = std::make_unique<SetColorCommand>(picked);
                g_history.ExecuteAndPush(std::move(cmd), g_state, hwnd, g_recording, &g_macro_recorded);
                UpdateMenu(hwnd);
            }
            break;
        }
        case IDM_PEN_1:
        case IDM_PEN_2:
        case IDM_PEN_3:
        case IDM_PEN_5: {
            int w = 1;
            if (id == IDM_PEN_2) w = 2;
            else if (id == IDM_PEN_3) w = 3;
            else if (id == IDM_PEN_5) w = 5;
            if (w != g_state.currentPenWidth) {
                auto cmd = std::make_unique<SetPenWidthCommand>(w);
                g_history.ExecuteAndPush(std::move(cmd), g_state, hwnd, g_recording, &g_macro_recorded);
                UpdateMenu(hwnd);
            }
            break;
        }
        case IDM_UNDO: {
            g_history.Undo(g_state, hwnd);
            UpdateMenu(hwnd);
            break;
        }
        case IDM_REDO: {
            g_history.Redo(g_state, hwnd);
            UpdateMenu(hwnd);
            break;
        }
        case IDM_MACRO_START: {
            g_recording = true;
            g_macro_recorded.clear();
            UpdateMenu(hwnd);
            break;
        }
        case IDM_MACRO_STOP: {
            g_recording = false;
            UpdateMenu(hwnd);
            break;
        }
        case IDM_MACRO_PLAY: {
            if (!g_macro_recorded.empty()) {
                auto macro = std::make_unique<MacroCommand>();
                for (const auto& c : g_macro_recorded)
                    macro->children.push_back(c->Clone());
                g_history.ExecuteAndPush(std::move(macro), g_state, hwnd, false, nullptr);
                UpdateMenu(hwnd);
            }
            break;
        }
        default:
            break;
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawAll(hdc, g_state);

        if (g_pendingShape != 0 && g_isDragging) {
            HPEN hPen = CreatePen(PS_DASH, std::max(1, g_state.currentPenWidth), g_state.currentColor);
            HGDIOBJ oldPen = SelectObject(hdc, hPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            RECT pr = g_currentRect;
            if (g_pendingShape == 2) {
                Rectangle(hdc, pr.left, pr.top, pr.right, pr.bottom);
            }
            else if (g_pendingShape == 3) {
                Ellipse(hdc, pr.left, pr.top, pr.right, pr.bottom);
            }
            else if (g_pendingShape == 1) {
                POINT pts[3];
                pts[0].x = (pr.left + pr.right) / 2; pts[0].y = pr.top;
                pts[1].x = pr.left; pts[1].y = pr.bottom;
                pts[2].x = pr.right; pts[2].y = pr.bottom;
                Polygon(hdc, pts, 3);
            }
            SelectObject(hdc, oldBrush);
            SelectObject(hdc, oldPen);
            DeleteObject(hPen);
        }
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"CommandPatternWinClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Паттерн Команда",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}