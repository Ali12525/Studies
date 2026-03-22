#pragma once
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <deque>

#pragma comment(lib, "Comdlg32.lib")

enum class ShapeType { Triangle, Rectangle, Ellipse };

struct Shape {
    int id;
    ShapeType type;
    RECT rc;
    COLORREF color;
    int penWidth;
};

struct AppState {
    std::vector<Shape> shapes;
    COLORREF currentColor = RGB(0, 0, 0);
    int currentPenWidth = 1;
    int nextShapeId = 1;
};

struct ICommand {
    virtual ~ICommand() = default;
    virtual void Execute(AppState& state, HWND hwnd) = 0;
    virtual void Unexecute(AppState& state, HWND hwnd) = 0;
    virtual std::unique_ptr<ICommand> Clone() const = 0;
};

class DrawShapeCommand : public ICommand {
public:
    ShapeType type;
    RECT rect;
    COLORREF color;
    int penWidth;
    int createdId = -1;

    DrawShapeCommand(ShapeType t, RECT rc, COLORREF c, int w);
    void Execute(AppState& state, HWND hwnd) override;
    void Unexecute(AppState& state, HWND hwnd) override;
    std::unique_ptr<ICommand> Clone() const override;
};

class SetColorCommand : public ICommand {
    COLORREF newColor;
    COLORREF oldColor;
    bool executed = false;
public:
    SetColorCommand(COLORREF c);
    void Execute(AppState& state, HWND hwnd) override;
    void Unexecute(AppState& state, HWND hwnd) override;
    std::unique_ptr<ICommand> Clone() const override;
};

class SetPenWidthCommand : public ICommand {
    int newWidth;
    int oldWidth;
    bool executed = false;
public:
    SetPenWidthCommand(int w);
    void Execute(AppState& state, HWND hwnd) override;
    void Unexecute(AppState& state, HWND hwnd) override;
    std::unique_ptr<ICommand> Clone() const override;
};

class MacroCommand : public ICommand {
public:
    std::vector<std::unique_ptr<ICommand>> children;
    void Execute(AppState& state, HWND hwnd) override;
    void Unexecute(AppState& state, HWND hwnd) override;
    std::unique_ptr<ICommand> Clone() const override;
};

class History {
    std::deque<std::unique_ptr<ICommand>> undoStack;
    std::deque<std::unique_ptr<ICommand>> redoStack;
    size_t limit = 10;
public:
    void ExecuteAndPush(std::unique_ptr<ICommand> cmd, AppState& state, HWND hwnd,
        bool recordForMacro, std::vector<std::unique_ptr<ICommand>>* macroRecorder);
    bool CanUndo() const { return !undoStack.empty(); }
    bool CanRedo() const { return !redoStack.empty(); }
    void Undo(AppState& state, HWND hwnd);
    void Redo(AppState& state, HWND hwnd);
};

RECT NormalizeRect(RECT rc);
void DrawAll(HDC hdc, const AppState& state);

#define IDM_SHAPE_TRIANGLE 101
#define IDM_SHAPE_RECT     102
#define IDM_SHAPE_ELLIPSE  103
#define IDM_COLOR_PICK     201
#define IDM_PEN_1          301
#define IDM_PEN_2          302
#define IDM_PEN_3          303
#define IDM_PEN_5          305
#define IDM_UNDO           401
#define IDM_REDO           402
#define IDM_MACRO_START    501
#define IDM_MACRO_STOP     502
#define IDM_MACRO_PLAY     503

class Lab1App {
public:
    static Lab1App* instance;

    Lab1App();
    ~Lab1App();

    bool Initialize(HINSTANCE hInstance);
    int Run();
    void Shutdown();

    static LRESULT CALLBACK WndProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE hInst_;
    HWND hwnd_;
    WNDCLASS wc_;

    AppState state_;
    History history_;
    bool recording_ = false;
    std::vector<std::unique_ptr<ICommand>> macro_recorded_;

    int selectedShape_ = 0;
    int pendingShape_ = 0;
    bool isDragging_ = false;
    POINT dragStart_ = { 0, 0 };
    RECT currentRect_ = { 0, 0, 0, 0 };
    const int MIN_SIZE = 10;

    LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    HMENU CreateAppMenu();
    void UpdateMenu();
    void UpdateWindowTitle();
    COLORREF PickColor(HWND hwnd, COLORREF initial);

    void OnShapeSelected(int shape);
    void OnColorPick();
    void OnPenWidth(int width);
    void OnUndo();
    void OnRedo();
    void OnMacroStart();
    void OnMacroStop();
    void OnMacroPlay();
    void OnLButtonDown(POINT pt);
    void OnMouseMove(POINT pt);
    void OnLButtonUp(POINT pt);
    void OnPaint();
};
