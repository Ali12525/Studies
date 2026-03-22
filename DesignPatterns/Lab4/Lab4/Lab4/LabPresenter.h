#pragma once
#include <memory>
#include "LabModel.h"
#include "IMainView.h"

class Lab1App;
class Lab2App;
class Lab3App;

class LabPresenter {
public:
    LabPresenter(std::shared_ptr<LabModel> model, 
                 std::shared_ptr<IMainView> mainView);
    
    ~LabPresenter() = default;

    void Run();

private:
    // Model и View
    std::shared_ptr<LabModel> model_;
    std::shared_ptr<IMainView> mainView_;

    bool running_;

    // Обработчики событий от View
    void OnLab1Selected();
    void OnLab2Selected();
    void OnLab3Selected();
    void OnExitSelected();

    // Вспомогательные методы
    void ShowLabMenu();
    void RunLab1();
    void RunLab2();
    void RunLab3();
};
