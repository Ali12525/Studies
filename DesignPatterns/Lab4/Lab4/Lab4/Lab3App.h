#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Decorator Pattern

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
    std::string getDescription() const override;
    std::string getPassport() const override;
    std::string getInsurance() const override;
    std::string getBankCard() const override;
};

class CardDecorator : public Card {
protected:
    std::unique_ptr<Card> card_;
public:
    CardDecorator(std::unique_ptr<Card> card);
};

class PassportDecorator : public CardDecorator {
    std::string series_;
    std::string number_;
public:
    PassportDecorator(std::unique_ptr<Card> card,
        const std::string& series,
        const std::string& number);
    std::string getDescription() const override;
    std::string getPassport() const override;
    std::string getInsurance() const override;
    std::string getBankCard() const override;
};

class InsuranceDecorator : public CardDecorator {
    std::string policyNumber_;
public:
    InsuranceDecorator(std::unique_ptr<Card> card, const std::string& policyNumber);
    std::string getDescription() const override;
    std::string getPassport() const override;
    std::string getInsurance() const override;
    std::string getBankCard() const override;
};

class BankCardDecorator : public CardDecorator {
    std::string cardNumber_;
public:
    BankCardDecorator(std::unique_ptr<Card> card, const std::string& cardNumber);
    std::string getDescription() const override;
    std::string getPassport() const override;
    std::string getInsurance() const override;
    std::string getBankCard() const override;
};

class Lab3App {
public:
    Lab3App();
    ~Lab3App();

    void Run();
    std::string GetFullDescription() const;
    std::vector<std::string> GetComponents() const;

private:
    std::unique_ptr<Card> card_;
    std::vector<std::string> components_;

    void BuildCard();
    void DisplayCardInfo();
    void DisplayDescription();
    void DisplayPassport();
    void DisplayInsurance();
    void DisplayBankCard();
};
