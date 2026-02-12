#include "SetPenWidthCommand.h"

SetPenWidthCommand::SetPenWidthCommand(int w) : newWidth(w), oldWidth(1) {}

// Выполняет команду: сохраняет старую толщину и устанавливает новую, инициирует перерисовку.
void SetPenWidthCommand::Execute(AppState& state, HWND hwnd) {
    oldWidth = state.currentPenWidth;
    state.currentPenWidth = newWidth;
    executed = true;
    InvalidateRect(hwnd, NULL, TRUE);
}

// Отменяет команду: восстанавливает старую толщину, если команда была выполнена.
void SetPenWidthCommand::Unexecute(AppState& state, HWND hwnd) {
    if (executed) {
        state.currentPenWidth = oldWidth;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

std::unique_ptr<ICommand> SetPenWidthCommand::Clone() const {
    return std::make_unique<SetPenWidthCommand>(newWidth);
}