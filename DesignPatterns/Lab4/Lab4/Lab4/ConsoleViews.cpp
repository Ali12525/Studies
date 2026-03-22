#include "ConsoleViews.h"
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

ConsoleMainView::ConsoleMainView()
    : onLab1Selected_(nullptr)
    , onLab2Selected_(nullptr)
    , onLab3Selected_(nullptr)
    , onExitSelected_(nullptr) {
}

void ConsoleMainView::SetOnLab1Selected(std::function<void()> handler) {
    onLab1Selected_ = handler;
}

void ConsoleMainView::SetOnLab2Selected(std::function<void()> handler) {
    onLab2Selected_ = handler;
}

void ConsoleMainView::SetOnLab3Selected(std::function<void()> handler) {
    onLab3Selected_ = handler;
}

void ConsoleMainView::SetOnExitSelected(std::function<void()> handler) {
    onExitSelected_ = handler;
}

void ConsoleMainView::ShowMenu() {
    std::cout << "======================================" << std::endl;
    std::cout << "  ГЛАВНОЕ МЕНЮ" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;
    std::cout << "  1. Лабораторная 1 (Команда)" << std::endl;
    std::cout << "  2. Лабораторная 2 (Одиночка)" << std::endl;
    std::cout << "  3. Лабораторная 3 (Декоратор)" << std::endl;
    std::cout << "  0. Выход" << std::endl;
    std::cout << std::endl;
    std::cout << "======================================" << std::endl;
}

void ConsoleMainView::ShowMessage(const std::string& msg) {
    std::cout << msg << std::endl;
}

void ConsoleMainView::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int ConsoleMainView::GetUserChoice() {
    int choice;
    std::cin >> choice;
    std::cin.ignore(10000, '\n');
    return choice;
}
