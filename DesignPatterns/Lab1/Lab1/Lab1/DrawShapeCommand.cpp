#include "DrawShapeCommand.h"
#include "DrawingUtils.h"

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