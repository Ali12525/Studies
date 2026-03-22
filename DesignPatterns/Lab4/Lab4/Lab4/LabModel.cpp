#include "LabModel.h"
#include "Lab1App.h"
#include "Lab2App.h"
#include "Lab3App.h"
#include <iostream>

LabModel::LabModel() {
    InitializeLabs();
}

LabModel::~LabModel() = default;

void LabModel::InitializeLabs() {
    labs_ = {
        {1, "Лабораторная 1", "Паттерн Команда", 
         "Графический редактор с фигурами, Отмена / Повтор, макросами", false},
        {2, "Лабораторная 2", "Паттерн Одиночка", 
         "Потокобезопасный логгер", false},
        {3, "Лабораторная 3", "Паттерн Декоратор", 
         "Универсальная электронная карта (паспорт, страховка, банк)", false}
    };
}

std::vector<LabInfo> LabModel::GetLabsList() const {
    return labs_;
}

LabInfo LabModel::GetLabInfo(int labId) const {
    for (const auto& lab : labs_) {
        if (lab.id == labId) {
            return lab;
        }
    }
    return {-1, "", "", "", false};
}

void LabModel::StartLab(int labId) {
    for (auto& lab : labs_) {
        if (lab.id == labId) {
            lab.isRunning = true;

            switch (labId) {
                case 1:
                    if (!lab1App_) {
                        lab1App_ = std::make_unique<Lab1App>();
                    }
                    break;
                case 2:
                    if (!lab2App_) {
                        lab2App_ = std::make_unique<Lab2App>();
                    }
                    break;
                case 3:
                    if (!lab3App_) {
                        lab3App_ = std::make_unique<Lab3App>();
                    }
                    break;
            }

            NotifyObservers();
            break;
        }
    }
}

void LabModel::StopLab(int labId) {
    for (auto& lab : labs_) {
        if (lab.id == labId) {
            lab.isRunning = false;
            NotifyObservers();
            break;
        }
    }
}

bool LabModel::IsLabRunning(int labId) const {
    for (const auto& lab : labs_) {
        if (lab.id == labId) {
            return lab.isRunning;
        }
    }
    return false;
}

Lab1App* LabModel::GetLab1App() const {
    return lab1App_.get();
}

Lab2App* LabModel::GetLab2App() const {
    return lab2App_.get();
}

Lab3App* LabModel::GetLab3App() const {
    return lab3App_.get();
}

void LabModel::AddObserver(ChangeCallback callback) {
    observers_.push_back(callback);
}

void LabModel::NotifyObservers() {
    for (auto& observer : observers_) {
        observer();
    }
}
