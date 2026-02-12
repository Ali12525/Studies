#pragma once
#include "Command.h"
#include "AppState.h"

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