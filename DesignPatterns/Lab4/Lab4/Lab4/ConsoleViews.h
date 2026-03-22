#pragma once
#include "IMainView.h"
#include <iostream>
#include <functional>
#include <cstdlib>

class ConsoleMainView : public IMainView {
public:
    ConsoleMainView();
    
    void SetOnLab1Selected(std::function<void()> handler) override;
    void SetOnLab2Selected(std::function<void()> handler) override;
    void SetOnLab3Selected(std::function<void()> handler) override;
    void SetOnExitSelected(std::function<void()> handler) override;
    
    void ShowMenu() override;
    void ShowMessage(const std::string& msg) override;
    void ClearScreen() override;
    int GetUserChoice() override;

private:
    std::function<void()> onLab1Selected_;
    std::function<void()> onLab2Selected_;
    std::function<void()> onLab3Selected_;
    std::function<void()> onExitSelected_;
};
