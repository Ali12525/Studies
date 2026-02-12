#include "SetColorCommand.h"

SetColorCommand::SetColorCommand(COLORREF c) : newColor(c), oldColor(RGB(0, 0, 0)) {}

// Выполняет команду: сохраняет старый цвет и устанавливает новый, инициирует перерисовку.
void SetColorCommand::Execute(AppState& state, HWND hwnd) {
    oldColor = state.currentColor;
    state.currentColor = newColor;
    executed = true;
    InvalidateRect(hwnd, NULL, TRUE);
}

// Отменяет команду: восстанавливает старый цвет, если команда была выполнена.
void SetColorCommand::Unexecute(AppState& state, HWND hwnd) {
    if (executed) {
        state.currentColor = oldColor;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

// Создаёт копию команды.
std::unique_ptr<ICommand> SetColorCommand::Clone() const {
    return std::make_unique<SetColorCommand>(newColor);
}