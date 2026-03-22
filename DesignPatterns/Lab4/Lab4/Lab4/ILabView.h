#pragma once
#include <string>
#include <functional>
#include <vector>

class ILabView {
public:
    virtual ~ILabView() = default;
    virtual void ShowHeader(const std::string& title) = 0;
    virtual void ShowFooter() = 0;
    virtual void WaitForKey() = 0;
};

class ILab1View : public ILabView {
public:
    virtual ~ILab1View() = default;
    virtual void ShowLaunchMessage() = 0;
    virtual void ShowEditorInfo() = 0;
};

class ILab2View : public ILabView {
public:
    virtual ~ILab2View() = default;
    virtual void ShowLogOutput(const std::vector<std::string>& logs) = 0;
    virtual void ShowLogStatus(bool isLogging) = 0;
};

class ILab3View : public ILabView {
public:
    virtual ~ILab3View() = default;
    virtual void ShowCardDescription(const std::string& description) = 0;
    virtual void ShowCardComponents(const std::vector<std::string>& components) = 0;
};
