#include <windows.h>
#include <iostream>
#include <memory>
#include <string>

class Card {
public:
    virtual ~Card() = default;
    virtual std::string getDescription() const = 0;
    virtual std::string getPassport() const = 0;
    virtual std::string getInsurance() const = 0;
    virtual std::string getBankCard() const = 0;
};

class BasicCard : public Card {
public:
    std::string getDescription() const override {
        return "Универсальная электронная карта";
    }

    std::string getPassport() const override {
        return "";
    }

    std::string getInsurance() const override {
        return "";
    }

    std::string getBankCard() const override {
        return "";
    }
};

class CardDecorator : public Card {
protected:
    std::unique_ptr<Card> card_;
public:
    CardDecorator(std::unique_ptr<Card> card) : card_(std::move(card)) {}
};

class PassportDecorator : public CardDecorator {
    std::string series_;
    std::string number_;
public:
    PassportDecorator(std::unique_ptr<Card> card,
        const std::string& series,
        const std::string& number)
        : CardDecorator(std::move(card)), series_(series), number_(number) {
    }

    std::string getDescription() const override {
        return card_->getDescription() + "\nПаспорт: серия " + series_ + " номер " + number_;
    }

    std::string getPassport() const override {
        return card_->getPassport() + "\nПаспорт: серия " + series_ + " номер " + number_;
    }

    std::string getInsurance() const override {
        return card_->getInsurance() + "";
    }

    std::string getBankCard() const override {
        return card_->getBankCard() + "";
    }
};

class InsuranceDecorator : public CardDecorator {
    std::string policyNumber_;
public:
    InsuranceDecorator(std::unique_ptr<Card> card, const std::string& policyNumber)
        : CardDecorator(std::move(card)), policyNumber_(policyNumber) {
    }

    std::string getDescription() const override {
        return card_->getDescription() + "\nСтраховой полис: № " + policyNumber_;
    }

    std::string getPassport() const override {
        return card_->getPassport() + "";
    }

    std::string getInsurance() const override {
        return card_->getInsurance() + "\nСтраховой полис: № " + policyNumber_;
    }

    std::string getBankCard() const override {
        return card_->getBankCard() + "";
    }
};

class BankCardDecorator : public CardDecorator {
    std::string cardNumber_;
public:
    BankCardDecorator(std::unique_ptr<Card> card, const std::string& cardNumber)
        : CardDecorator(std::move(card)), cardNumber_(cardNumber) {
    }

    std::string getDescription() const override {
        return card_->getDescription() + "\nБанковская карта: номер " + cardNumber_;
    }

    std::string getPassport() const override {
        return card_->getPassport() + "";
    }

    std::string getInsurance() const override {
        return card_->getInsurance() + "";
    }

    std::string getBankCard() const override {
        return card_->getBankCard() + "\nБанковская карта: номер " + cardNumber_;
    }
};

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    auto basic = std::make_unique<BasicCard>();
    std::cout << "Базовая карта:\n" << basic->getDescription() << "\n\n";

    auto withPassport = std::make_unique<PassportDecorator>(std::move(basic), "", "");
    std::cout << "С паспортом:\n" << withPassport->getPassport() << "\n\n";

    auto withPassportAndInsurance = std::make_unique<InsuranceDecorator>(std::move(withPassport), "");
    std::cout << "С паспортом и страховкой:\n" << withPassportAndInsurance->getInsurance() << "\n\n";

    auto fullCard = std::make_unique<BankCardDecorator>(std::move(withPassportAndInsurance), "");
    std::cout << "Полная карта (паспорт + страховка + банк):\n" << fullCard->getBankCard() << "\n";

    return 0;
}