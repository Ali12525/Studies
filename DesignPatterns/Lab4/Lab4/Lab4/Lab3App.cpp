#include "Lab3App.h"

#ifdef _WIN32
#include <windows.h>
#endif

std::string BasicCard::getDescription() const {
    return "Универсальная электронная карта";
}

std::string BasicCard::getPassport() const {
    return "";
}

std::string BasicCard::getInsurance() const {
    return "";
}

std::string BasicCard::getBankCard() const {
    return "";
}

CardDecorator::CardDecorator(std::unique_ptr<Card> card)
    : card_(std::move(card)) {
}

PassportDecorator::PassportDecorator(std::unique_ptr<Card> card,
    const std::string& series,
    const std::string& number)
    : CardDecorator(std::move(card)), series_(series), number_(number) {
}

std::string PassportDecorator::getDescription() const {
    std::string base = card_->getDescription();
    if (!series_.empty() || !number_.empty()) {
        return base + "\n  + Паспорт: серия " + series_ + " номер " + number_;
    }
    return base;
}

std::string PassportDecorator::getPassport() const {
    std::string base = card_->getPassport();
    if (!series_.empty() || !number_.empty()) {
        if (!base.empty()) base += "\n";
        return base + "Паспорт: серия " + series_ + " номер " + number_;
    }
    return base;
}

std::string PassportDecorator::getInsurance() const {
    return card_->getInsurance();
}

std::string PassportDecorator::getBankCard() const {
    return card_->getBankCard();
}

InsuranceDecorator::InsuranceDecorator(std::unique_ptr<Card> card, const std::string& policyNumber)
    : CardDecorator(std::move(card)), policyNumber_(policyNumber) {
}

std::string InsuranceDecorator::getDescription() const {
    std::string base = card_->getDescription();
    if (!policyNumber_.empty()) {
        return base + "\n  + Страховой полис: " + policyNumber_;
    }
    return base;
}

std::string InsuranceDecorator::getPassport() const {
    return card_->getPassport();
}

std::string InsuranceDecorator::getInsurance() const {
    std::string base = card_->getInsurance();
    if (!policyNumber_.empty()) {
        if (!base.empty()) base += "\n";
        return base + "Страховой полис: " + policyNumber_;
    }
    return base;
}

std::string InsuranceDecorator::getBankCard() const {
    return card_->getBankCard();
}

BankCardDecorator::BankCardDecorator(std::unique_ptr<Card> card, const std::string& cardNumber)
    : CardDecorator(std::move(card)), cardNumber_(cardNumber) {
}

std::string BankCardDecorator::getDescription() const {
    std::string base = card_->getDescription();
    if (!cardNumber_.empty()) {
        return base + "\n  + Банковская карта: номер " + cardNumber_;
    }
    return base;
}

std::string BankCardDecorator::getPassport() const {
    return card_->getPassport();
}

std::string BankCardDecorator::getInsurance() const {
    return card_->getInsurance();
}

std::string BankCardDecorator::getBankCard() const {
    std::string base = card_->getBankCard();
    if (!cardNumber_.empty()) {
        if (!base.empty()) base += "\n";
        return base + "Банковская карта: номер " + cardNumber_;
    }
    return base;
}


Lab3App::Lab3App() {
}

Lab3App::~Lab3App() {
}

void Lab3App::BuildCard() {
    auto basic = std::make_unique<BasicCard>();

    auto withPassport = std::make_unique<PassportDecorator>(
        std::move(basic), "", "");
    components_.push_back("Паспорт");

    auto withInsurance = std::make_unique<InsuranceDecorator>(
        std::move(withPassport), "");
    components_.push_back("Страховой полис");

    card_ = std::make_unique<BankCardDecorator>(
        std::move(withInsurance), "");
    components_.push_back("Банковская карта");
}

void Lab3App::DisplayCardInfo() {
    std::cout << "ЛАБОРАТОРНАЯ 3: ПАТТЕРН ДЕКОРАТОР" << std::endl;
    std::cout << "(Универсальная электронная карта)" << std::endl;
    std::cout << std::endl;
    std::cout << "Информация о карте:" << std::endl;
    std::cout << "─────────────────────────────────────────────────────────────" << std::endl;
    std::cout << card_->getDescription() << std::endl;
    std::cout << "─────────────────────────────────────────────────────────────" << std::endl;

    std::cout << std::endl;
    std::cout << "Доступ к отдельным компонентам:" << std::endl;
    DisplayInsurance();
    DisplayPassport();
    DisplayBankCard();
    }

void Lab3App::DisplayPassport() {
    std::cout << "  Паспорт:    " << (card_->getPassport().empty() ? "<не добавлен>" : card_->getPassport()) << std::endl;
}
void Lab3App::DisplayInsurance() {
    std::cout << "  Страховка:  " << (card_->getInsurance().empty() ? "<не добавлен>" : card_->getInsurance()) << std::endl;
}
void Lab3App::DisplayBankCard() {
    std::cout << "  Банк карта: " << (card_->getBankCard().empty() ? "<не добавлен>" : card_->getBankCard()) << std::endl;
}

std::string Lab3App::GetFullDescription() const {
    if (card_) {
        return card_->getDescription();
    }
    return "";
}

std::vector<std::string> Lab3App::GetComponents() const {
    return components_;
}

void Lab3App::Run() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    BuildCard();
    DisplayCardInfo();
}
