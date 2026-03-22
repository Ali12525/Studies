#include "LabPresenter.h"
#include "Lab1App.h"
#include "Lab2App.h"
#include "Lab3App.h"
#include <iostream>
#include <windows.h>

LabPresenter::LabPresenter(std::shared_ptr<LabModel> model, std::shared_ptr<IMainView> mainView)
    : model_(model)
    , mainView_(mainView)
    , running_(true) {
    
    // Регистрация обработчиков событий
    mainView_->SetOnLab1Selected([this]() { OnLab1Selected(); });
    mainView_->SetOnLab2Selected([this]() { OnLab2Selected(); });
    mainView_->SetOnLab3Selected([this]() { OnLab3Selected(); });
    mainView_->SetOnExitSelected([this]() { OnExitSelected(); });

    // Подписка на изменения модели
    model_->AddObserver([this]() {});
}

void LabPresenter::Run() {
    mainView_->ShowMessage("Меню:");
    mainView_->ShowMessage("  1. Паттерн Команда - Графический редактор");
    mainView_->ShowMessage("  2. Паттерн Одиночка - Потокобезопасный логгер");
    mainView_->ShowMessage("  3. Паттерн Декоратор - Универсальная электронная карта");
    mainView_->ShowMessage("");

    while (running_) {
        ShowLabMenu();
        
        int choice = mainView_->GetUserChoice();
        
        switch (choice) {
            case 1:
                OnLab1Selected();
                break;
            case 2:
                OnLab2Selected();
                break;
            case 3:
                OnLab3Selected();
                break;
            case 0:
                OnExitSelected();
                break;
            default:
                mainView_->ShowMessage("Неверный выбор. Попробуйте снова.");
                break;
        }
    }
}

void LabPresenter::ShowLabMenu() {
    mainView_->ClearScreen();
    mainView_->ShowMessage("");
    mainView_->ShowMessage("======================================");
    mainView_->ShowMessage("  ГЛАВНОЕ МЕНЮ");
    mainView_->ShowMessage("======================================");
    mainView_->ShowMessage("");
    
    auto labs = model_->GetLabsList();
    for (const auto& lab : labs) {
        std::string status = lab.isRunning ? " [ЗАПУЩЕНА]" : "";
        std::cout << "  " << lab.id << ". " << lab.name 
                  << " (" << lab.pattern << ")" << status << std::endl;
        std::cout << "     " << lab.description << std::endl;
        std::cout << std::endl;
    }
    
    mainView_->ShowMessage("  0. Выход");
    mainView_->ShowMessage("");
    mainView_->ShowMessage("======================================");
    std::cout << "Выберите лабораторную работу: ";
}

void LabPresenter::OnLab1Selected() {
    model_->StartLab(1);
    RunLab1();
    model_->StopLab(1);
}

void LabPresenter::OnLab2Selected() {
    model_->StartLab(2);
    RunLab2();
    model_->StopLab(2);
}

void LabPresenter::OnLab3Selected() {
    model_->StartLab(3);
    RunLab3();
    model_->StopLab(3);
}

void LabPresenter::OnExitSelected() {
    mainView_->ShowMessage("");
    mainView_->ShowMessage("Завершение работы...");
    running_ = false;
}

void LabPresenter::RunLab1() {
    mainView_->ShowMessage("");
    mainView_->ShowMessage("ЗАПУСК ЛАБОРАТОРНОЙ 1: Графический редактор");
    mainView_->ShowMessage("");

    Lab1App* app = model_->GetLab1App();
    if (!app) {
        mainView_->ShowMessage("Ошибка: не удалось создать приложение");
        return;
    }

    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    if (!app->Initialize(hInstance)) {
        mainView_->ShowMessage("Ошибка инициализации приложения");
        return;
    }

    mainView_->ShowMessage("Графический редактор запущен!");
    mainView_->ShowMessage("Функционал:");
    mainView_->ShowMessage("  - Рисование: треугольник, прямоугольник, эллипс");
    mainView_->ShowMessage("  - Выбор цвета и толщины пера");
    mainView_->ShowMessage("  - Повтор / Отмена");
    mainView_->ShowMessage("  - Запись и воспроизведение макросов");
    mainView_->ShowMessage("");
    mainView_->ShowMessage("Закройте окно редактора для возврата в меню...");
    mainView_->ShowMessage("");

    app->Run();

    // После закрытия окна возвращаемся в меню
    app->Shutdown();
}

void LabPresenter::RunLab2() {
    mainView_->ShowMessage("");
    mainView_->ShowMessage("ЗАПУСК ЛАБОРАТОРНОЙ 2: Логгер (Singleton)");
    mainView_->ShowMessage("");

    Lab2App* app = model_->GetLab2App();
    if (!app) {
        mainView_->ShowMessage("Ошибка: не удалось создать приложение");
        return;
    }

    app->SetLogFile("lab2_output.log");
    app->Run();

    mainView_->ShowMessage("");
    mainView_->ShowMessage("Нажмите Enter для возврата в меню...");
    std::cin.get();
}

void LabPresenter::RunLab3() {
    mainView_->ShowMessage("");
    mainView_->ShowMessage("ЗАПУСК ЛАБОРАТОРНОЙ 3: Декоратор (Электронная карта)");
    mainView_->ShowMessage("");

    Lab3App* app = model_->GetLab3App();
    if (!app) {
        mainView_->ShowMessage("Ошибка: не удалось создать приложение");
        return;
    }

    app->Run();

    mainView_->ShowMessage("");
    mainView_->ShowMessage("Нажмите Enter для возврата в меню...");
    std::cin.get();
}