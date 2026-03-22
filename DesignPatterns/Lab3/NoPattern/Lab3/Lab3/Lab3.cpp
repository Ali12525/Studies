#include <windows.h>
#include <iostream>
#include <string>

class UniversalCard {
private:
    bool hasPassport_ = false;
    bool hasInsurance_ = false;
    bool hasBankCard_ = false;

    std::string passportSeries_;
    std::string passportNumber_;
    std::string insuranceNumber_;
    std::string bankCardNumber_;

public:
    void addPassport(const std::string& series, const std::string& number) {
        hasPassport_ = true;
        passportSeries_ = series;
        passportNumber_ = number;
    }

    void addInsurance(const std::string& number) {
        hasInsurance_ = true;
        insuranceNumber_ = number;
    }

    void addBankCard(const std::string& number) {
        hasBankCard_ = true;
        bankCardNumber_ = number;
    }

    std::string getDescription() const {
        std::string result = "Универсальная электронная карта";
        if (hasPassport_) {
            result += "\nПаспорт: серия " + passportSeries_ + " номер " + passportNumber_;
        }
        if (hasInsurance_) {
            result += "\nСтраховой полис: № " + insuranceNumber_;
        }
        if (hasBankCard_) {
            result += "\nБанковская карта: номер " + bankCardNumber_;
        }
        return result;
    }
};

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    UniversalCard basicCard;
    std::cout << "Базовая карта:\n" << basicCard.getDescription() << "\n\n";

    UniversalCard cardWithPassport;
    cardWithPassport.addPassport("", "");
    std::cout << "С паспортом:\n" << cardWithPassport.getDescription() << "\n\n";

    UniversalCard cardWithPassportAndInsurance;
    cardWithPassportAndInsurance.addPassport("", "");
    cardWithPassportAndInsurance.addInsurance("");
    std::cout << "С паспортом и страховкой:\n" << cardWithPassportAndInsurance.getDescription() << "\n\n";

    UniversalCard fullCard;
    fullCard.addPassport("", "");
    fullCard.addInsurance("");
    fullCard.addBankCard("");
    std::cout << "Полная карта (паспорт + страховка + банк):\n" << fullCard.getDescription() << "\n";

    return 0;
}