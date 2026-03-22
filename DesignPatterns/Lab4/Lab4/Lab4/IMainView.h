#pragma once
#include <string>
#include <functional>

class IMainView {
public:
    virtual ~IMainView() = default;

    // Установка обработчиков событий
    virtual void SetOnLab1Selected(std::function<void()> handler) = 0;
    virtual void SetOnLab2Selected(std::function<void()> handler) = 0;
    virtual void SetOnLab3Selected(std::function<void()> handler) = 0;
    virtual void SetOnExitSelected(std::function<void()> handler) = 0;
    // Отображение
    virtual void ShowMenu() = 0;
    virtual void ShowMessage(const std::string& msg) = 0;
    virtual void ClearScreen() = 0;
    virtual int GetUserChoice() = 0;
};
