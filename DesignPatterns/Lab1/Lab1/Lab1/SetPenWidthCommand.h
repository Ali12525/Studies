#pragma once
#include "Command.h"
#include "AppState.h"

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