#pragma once
#include <deque>
#include <memory>
#include "Command.h"
#include "AppState.h"

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