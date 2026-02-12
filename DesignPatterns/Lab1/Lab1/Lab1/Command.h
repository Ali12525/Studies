#pragma once
#define NOMINMAX
#include <windows.h>
#include <memory>

struct AppState;

struct ICommand {
    virtual ~ICommand() = default;
    virtual void Execute(AppState& state, HWND hwnd) = 0;
    virtual void Unexecute(AppState& state, HWND hwnd) = 0;
    virtual std::unique_ptr<ICommand> Clone() const = 0;
};