#pragma once
#include "Command.h"
#include <vector>

class MacroCommand : public ICommand {
public:
    std::vector<std::unique_ptr<ICommand>> children;

    void Execute(AppState& state, HWND hwnd) override;
    void Unexecute(AppState& state, HWND hwnd) override;
    std::unique_ptr<ICommand> Clone() const override;
};