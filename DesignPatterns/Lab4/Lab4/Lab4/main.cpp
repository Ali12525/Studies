#include <iostream>
#include <memory>
#include <windows.h>

#include "LabModel.h"
#include "LabPresenter.h"
#include "ConsoleViews.h"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    auto model = std::make_shared<LabModel>();
    auto mainView = std::make_shared<ConsoleMainView>();
    auto presenter = std::make_shared<LabPresenter>(model, mainView);

    presenter->Run();

    return 0;
}