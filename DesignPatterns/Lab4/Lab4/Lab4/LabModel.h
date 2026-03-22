#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>

class Lab1App;
class Lab2App;
class Lab3App;

struct LabInfo {
    int id;
    std::string name;
    std::string pattern;
    std::string description;
    bool isRunning;
};

class LabModel {
public:
    LabModel();
    ~LabModel();

    std::vector<LabInfo> GetLabsList() const;

    LabInfo GetLabInfo(int labId) const;
    void StartLab(int labId);
    void StopLab(int labId);
    bool IsLabRunning(int labId) const;

    Lab1App* GetLab1App() const;
    Lab2App* GetLab2App() const;
    Lab3App* GetLab3App() const;

    // Подписка на изменения
    using ChangeCallback = std::function<void()>;
    void AddObserver(ChangeCallback callback);
    void NotifyObservers();

private:
    std::vector<LabInfo> labs_;
    std::vector<ChangeCallback> observers_;

    std::unique_ptr<Lab1App> lab1App_;
    std::unique_ptr<Lab2App> lab2App_;
    std::unique_ptr<Lab3App> lab3App_;

    void InitializeLabs();
};
