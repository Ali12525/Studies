#include "Lab1App.h"
#include <commdlg.h>
#include <string>

#pragma comment(lib, "Comdlg32.lib")

using namespace std;

Lab1App* Lab1App::instance = nullptr;

DrawShapeCommand::DrawShapeCommand(ShapeType t, RECT rc, COLORREF c, int w)
    : type(t), rect(NormalizeRect(rc)), color(c), penWidth(w) {
}

void DrawShapeCommand::Execute(AppState& state, HWND hwnd) {
    Shape s;
    s.id = state.nextShapeId++;
    s.type = type;
    s.rc = rect;
    s.color = color;
    s.penWidth = penWidth;
    state.shapes.push_back(s);
    createdId = s.id;
    InvalidateRect(hwnd, NULL, TRUE);
}

void DrawShapeCommand::Unexecute(AppState& state, HWND hwnd) {
    auto it = std::find_if(state.shapes.begin(), state.shapes.end(),
        [&](const Shape& sh) { return sh.id == createdId; });
    if (it != state.shapes.end()) {
        state.shapes.erase(it);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

std::unique_ptr<ICommand> DrawShapeCommand::Clone() const {
    return std::make_unique<DrawShapeCommand>(type, rect, color, penWidth);
}

SetColorCommand::SetColorCommand(COLORREF c) : newColor(c), oldColor(0) {}

void SetColorCommand::Execute(AppState& state, HWND hwnd) {
    oldColor = state.currentColor;
    state.currentColor = newColor;
    executed = true;
}

void SetColorCommand::Unexecute(AppState& state, HWND hwnd) {
    if (executed) {
        state.currentColor = oldColor;
    }
}

std::unique_ptr<ICommand> SetColorCommand::Clone() const {
    return std::make_unique<SetColorCommand>(newColor);
}

SetPenWidthCommand::SetPenWidthCommand(int w) : newWidth(w), oldWidth(0) {}

void SetPenWidthCommand::Execute(AppState& state, HWND hwnd) {
    oldWidth = state.currentPenWidth;
    state.currentPenWidth = newWidth;
    executed = true;
}

void SetPenWidthCommand::Unexecute(AppState& state, HWND hwnd) {
    if (executed) {
        state.currentPenWidth = oldWidth;
    }
}

std::unique_ptr<ICommand> SetPenWidthCommand::Clone() const {
    return std::make_unique<SetPenWidthCommand>(newWidth);
}

void MacroCommand::Execute(AppState& state, HWND hwnd) {
    for (auto& cmd : children) {
        cmd->Execute(state, hwnd);
    }
}

void MacroCommand::Unexecute(AppState& state, HWND hwnd) {
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        (*it)->Unexecute(state, hwnd);
    }
}

std::unique_ptr<ICommand> MacroCommand::Clone() const {
    auto clone = std::make_unique<MacroCommand>();
    for (const auto& c : children) {
        clone->children.push_back(c->Clone());
    }
    return clone;
}

void History::ExecuteAndPush(std::unique_ptr<ICommand> cmd, AppState& state, HWND hwnd,
    bool recordForMacro, std::vector<std::unique_ptr<ICommand>>* macroRecorder) {
    cmd->Execute(state, hwnd);
    undoStack.push_back(std::move(cmd));
    if (undoStack.size() > limit) undoStack.pop_front();
    redoStack.clear();
    if (recordForMacro && macroRecorder) {
        macroRecorder->push_back(undoStack.back()->Clone());
    }
}

void History::Undo(AppState& state, HWND hwnd) {
    if (!undoStack.empty()) {
        auto cmd = std::move(undoStack.back());
        undoStack.pop_back();
        cmd->Unexecute(state, hwnd);
        redoStack.push_back(std::move(cmd));
    }
}

void History::Redo(AppState& state, HWND hwnd) {
    if (!redoStack.empty()) {
        auto cmd = std::move(redoStack.back());
        redoStack.pop_back();
        cmd->Execute(state, hwnd);
        undoStack.push_back(std::move(cmd));
    }
}

RECT NormalizeRect(RECT rc) {
    RECT result;
    result.left = min(rc.left, rc.right);
    result.right = max(rc.left, rc.right);
    result.top = min(rc.top, rc.bottom);
    result.bottom = max(rc.top, rc.bottom);
    return result;
}

void DrawAll(HDC hdc, const AppState& state) {
    for (const auto& s : state.shapes) {
        HPEN hPen = CreatePen(PS_SOLID, s.penWidth, s.color);
        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        if (s.type == ShapeType::Rectangle) {
            Rectangle(hdc, s.rc.left, s.rc.top, s.rc.right, s.rc.bottom);
        }
        else if (s.type == ShapeType::Ellipse) {
            Ellipse(hdc, s.rc.left, s.rc.top, s.rc.right, s.rc.bottom);
        }
        else if (s.type == ShapeType::Triangle) {
            POINT pts[3];
            pts[0].x = (s.rc.left + s.rc.right) / 2; pts[0].y = s.rc.top;
            pts[1].x = s.rc.left; pts[1].y = s.rc.bottom;
            pts[2].x = s.rc.right; pts[2].y = s.rc.bottom;
            Polygon(hdc, pts, 3);
        }

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
    }
}

Lab1App::Lab1App() : hInst_(nullptr), hwnd_(nullptr) {
    instance = this;
    ZeroMemory(&wc_, sizeof(wc_));
}

Lab1App::~Lab1App() {
    instance = nullptr;
}

bool Lab1App::Initialize(HINSTANCE hInstance) {
    hInst_ = hInstance;

    const wchar_t CLASS_NAME[] = L"Lab1GraphicsEditorClass";

    wc_.lpfnWndProc = WndProcStatic;
    wc_.hInstance = hInst_;
    wc_.lpszClassName = CLASS_NAME;
    wc_.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc_)) {
        return false;
    }

    hwnd_ = CreateWindowEx(
        0, CLASS_NAME, L"Лабораторная 1: Графический редактор (Паттерн Команда)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        NULL, NULL, hInst_, NULL
    );

    if (!hwnd_) {
        return false;
    }

    SetMenu(hwnd_, CreateAppMenu());
    UpdateMenu();
    UpdateWindowTitle();

    return true;
}

int Lab1App::Run() {
    if (!hwnd_) return 0;

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void Lab1App::Shutdown() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
}

HMENU Lab1App::CreateAppMenu() {
    HMENU hMenu = CreateMenu();

    HMENU menuShapes = CreateMenu();
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_TRIANGLE, L"Треугольник\tCtrl+1");
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_RECT, L"Прямоугольник\tCtrl+2");
    AppendMenu(menuShapes, MF_STRING, IDM_SHAPE_ELLIPSE, L"Эллипс\tCtrl+3");

    HMENU menuParams = CreateMenu();
    AppendMenu(menuParams, MF_STRING, IDM_COLOR_PICK, L"Выбрать цвет...\tCtrl+C");
    AppendMenu(menuParams, MF_SEPARATOR, 0, NULL);
    AppendMenu(menuParams, MF_STRING, IDM_PEN_1, L"Толщина 1\tCtrl+Shift+1");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_2, L"Толщина 2\tCtrl+Shift+2");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_3, L"Толщина 3\tCtrl+Shift+3");
    AppendMenu(menuParams, MF_STRING, IDM_PEN_5, L"Толщина 5\tCtrl+Shift+5");

    HMENU menuEdit = CreateMenu();
    AppendMenu(menuEdit, MF_STRING, IDM_UNDO, L"Отмена\tCtrl+Z");
    AppendMenu(menuEdit, MF_STRING, IDM_REDO, L"Повтор\tCtrl+Y");

    HMENU menuMacro = CreateMenu();
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_START, L"Начать запись\tCtrl+R");
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_STOP, L"Остановить запись\tCtrl+S");
    AppendMenu(menuMacro, MF_STRING, IDM_MACRO_PLAY, L"Выполнить макрокоманду\tCtrl+P");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuShapes, L"Фигуры");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuParams, L"Параметры");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuEdit, L"Правка");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)menuMacro, L"Макро");

    return hMenu;
}

void Lab1App::UpdateMenu() {
    HMENU hMenu = GetMenu(hwnd_);
    EnableMenuItem(hMenu, IDM_UNDO, MF_BYCOMMAND | (history_.CanUndo() ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_REDO, MF_BYCOMMAND | (history_.CanRedo() ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_MACRO_STOP, MF_BYCOMMAND | (recording_ ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(hMenu, IDM_MACRO_START, MF_BYCOMMAND | (recording_ ? MF_GRAYED : MF_ENABLED));
    EnableMenuItem(hMenu, IDM_MACRO_PLAY, MF_BYCOMMAND | (!macro_recorded_.empty() ? MF_ENABLED : MF_GRAYED));
    DrawMenuBar(hwnd_);
}

void Lab1App::UpdateWindowTitle() {
    const wchar_t* base = L"Лабораторная 1: Паттерн Команда";
    wchar_t buf[256];
    if (pendingShape_ != 0) {
        const wchar_t* sname = L"";
        if (pendingShape_ == 1) sname = L"Треугольник";
        else if (pendingShape_ == 2) sname = L"Прямоугольник";
        else if (pendingShape_ == 3) sname = L"Эллипс";
        swprintf_s(buf, L"%s — %s: перетащите для рисования (Esc для отмены)", base, sname);
    }
    else if (selectedShape_ != 0) {
        const wchar_t* sname = L"";
        if (selectedShape_ == 1) sname = L"Треугольник";
        else if (selectedShape_ == 2) sname = L"Прямоугольник";
        else if (selectedShape_ == 3) sname = L"Эллипс";
        swprintf_s(buf, L"%s — Выбрано: %s (выберите снова для рисования)", base, sname);
    }
    else {
        swprintf_s(buf, L"%s", base);
    }
    SetWindowText(hwnd_, buf);
}

COLORREF Lab1App::PickColor(HWND hwnd, COLORREF initial) {
    CHOOSECOLOR cc = { sizeof(cc) };
    static COLORREF cust[16] = { 0 };
    cc.hwndOwner = hwnd;
    cc.lpCustColors = cust;
    cc.rgbResult = initial;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    if (ChooseColor(&cc)) return cc.rgbResult;
    return initial;
}

void Lab1App::OnShapeSelected(int shape) {
    selectedShape_ = shape;
    pendingShape_ = shape;
    UpdateWindowTitle();
}

void Lab1App::OnColorPick() {
    COLORREF picked = PickColor(hwnd_, state_.currentColor);
    if (picked != state_.currentColor) {
        auto cmd = std::make_unique<SetColorCommand>(picked);
        history_.ExecuteAndPush(std::move(cmd), state_, hwnd_, recording_, &macro_recorded_);
        UpdateMenu();
    }
}

void Lab1App::OnPenWidth(int width) {
    if (width != state_.currentPenWidth) {
        auto cmd = std::make_unique<SetPenWidthCommand>(width);
        history_.ExecuteAndPush(std::move(cmd), state_, hwnd_, recording_, &macro_recorded_);
        UpdateMenu();
    }
}

void Lab1App::OnUndo() {
    history_.Undo(state_, hwnd_);
    UpdateMenu();
}

void Lab1App::OnRedo() {
    history_.Redo(state_, hwnd_);
    UpdateMenu();
}

void Lab1App::OnMacroStart() {
    recording_ = true;
    macro_recorded_.clear();
    UpdateMenu();
}

void Lab1App::OnMacroStop() {
    recording_ = false;
    UpdateMenu();
}

void Lab1App::OnMacroPlay() {
    if (!macro_recorded_.empty()) {
        auto macro = std::make_unique<MacroCommand>();
        for (const auto& c : macro_recorded_)
            macro->children.push_back(c->Clone());
        history_.ExecuteAndPush(std::move(macro), state_, hwnd_, false, nullptr);
        UpdateMenu();
    }
}

void Lab1App::OnLButtonDown(POINT pt) {
    if (pendingShape_ != 0 && !isDragging_) {
        isDragging_ = true;
        dragStart_ = pt;
        currentRect_.left = pt.x;
        currentRect_.top = pt.y;
        currentRect_.right = pt.x;
        currentRect_.bottom = pt.y;
        SetCapture(hwnd_);
        InvalidateRect(hwnd_, NULL, TRUE);
    }
}

void Lab1App::OnMouseMove(POINT pt) {
    if (isDragging_) {
        currentRect_.left = dragStart_.x;
        currentRect_.top = dragStart_.y;
        currentRect_.right = pt.x;
        currentRect_.bottom = pt.y;
        currentRect_ = NormalizeRect(currentRect_);
        InvalidateRect(hwnd_, NULL, TRUE);
    }
}

void Lab1App::OnLButtonUp(POINT pt) {
    if (isDragging_) {
        RECT finalRect = { dragStart_.x, dragStart_.y, pt.x, pt.y };
        finalRect = NormalizeRect(finalRect);

        if ((finalRect.right - finalRect.left) < MIN_SIZE)
            finalRect.right = finalRect.left + MIN_SIZE;
        if ((finalRect.bottom - finalRect.top) < MIN_SIZE)
            finalRect.bottom = finalRect.top + MIN_SIZE;

        ShapeType t = ShapeType::Rectangle;
        if (pendingShape_ == 1) t = ShapeType::Triangle;
        else if (pendingShape_ == 2) t = ShapeType::Rectangle;
        else if (pendingShape_ == 3) t = ShapeType::Ellipse;

        auto cmd = std::make_unique<DrawShapeCommand>(t, finalRect, state_.currentColor, state_.currentPenWidth);
        history_.ExecuteAndPush(std::move(cmd), state_, hwnd_, recording_, &macro_recorded_);

        isDragging_ = false;
        ReleaseCapture();
        if (selectedShape_ != 0) pendingShape_ = selectedShape_;
        else pendingShape_ = 0;

        UpdateMenu();
        UpdateWindowTitle();
        InvalidateRect(hwnd_, NULL, TRUE);
    }
}

void Lab1App::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    DrawAll(hdc, state_);

    if (pendingShape_ != 0 && isDragging_) {
        HPEN hPen = CreatePen(PS_DASH, max(1, state_.currentPenWidth), state_.currentColor);
        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        RECT pr = currentRect_;
        if (pendingShape_ == 2) {
            Rectangle(hdc, pr.left, pr.top, pr.right, pr.bottom);
        }
        else if (pendingShape_ == 3) {
            Ellipse(hdc, pr.left, pr.top, pr.right, pr.bottom);
        }
        else if (pendingShape_ == 1) {
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
    EndPaint(hwnd_, &ps);
}

LRESULT CALLBACK Lab1App::WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (instance) {
        return instance->WndProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Lab1App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (isDragging_) {
                isDragging_ = false;
                ReleaseCapture();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (pendingShape_ != 0) {
                pendingShape_ = 0;
                UpdateWindowTitle();
            }
        }
        return 0;

    case WM_LBUTTONDOWN:
        OnLButtonDown({ LOWORD(lParam), HIWORD(lParam) });
        return 0;

    case WM_MOUSEMOVE:
        OnMouseMove({ LOWORD(lParam), HIWORD(lParam) });
        return 0;

    case WM_LBUTTONUP:
        OnLButtonUp({ LOWORD(lParam), HIWORD(lParam) });
        return 0;

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDM_SHAPE_TRIANGLE: OnShapeSelected(1); break;
        case IDM_SHAPE_RECT: OnShapeSelected(2); break;
        case IDM_SHAPE_ELLIPSE: OnShapeSelected(3); break;
        case IDM_COLOR_PICK: OnColorPick(); break;
        case IDM_PEN_1: OnPenWidth(1); break;
        case IDM_PEN_2: OnPenWidth(2); break;
        case IDM_PEN_3: OnPenWidth(3); break;
        case IDM_PEN_5: OnPenWidth(5); break;
        case IDM_UNDO: OnUndo(); break;
        case IDM_REDO: OnRedo(); break;
        case IDM_MACRO_START: OnMacroStart(); break;
        case IDM_MACRO_STOP: OnMacroStop(); break;
        case IDM_MACRO_PLAY: OnMacroPlay(); break;
        }
        return 0;
    }

    case WM_PAINT:
        OnPaint();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}