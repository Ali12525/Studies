#pragma once
#include "Command.h"
#include "AppState.h"

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