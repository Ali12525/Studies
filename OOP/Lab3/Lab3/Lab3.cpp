#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

class Organization
{
private:
    string organization;
    string date;
public:
    string getOrganization() { return organization; }

    void setOrganization(string organization) { this->organization = organization; }

    string getDate() { return date; }

    void setDate(string date) { this->date = date; }

    void showOrganization()
    {
        cout << "Организация: " << getOrganization() << endl;
    }

    void showDate() 
    {
        cout << "Дата создания: " << getDate() << endl;
    }
};

class InsuranceCompany : public Organization
{
private:
    string insuranceCompany;
public:
    string getInsuranceCompany() { return insuranceCompany; }

    void setInsuranceCompany(string insuranceCompany) { this->insuranceCompany = insuranceCompany; }

    void showInsuranceCompany()
    {
        cout << "Страховая компания: " << getInsuranceCompany() << endl;
    }
};

class Factory : public InsuranceCompany
{
private:
    string nameFactory;
    int performance;
public:
    Factory(string organization, string insuranceCompany, string nameFactory, int performance, string date)
    {
        setOrganization(organization);
        setInsuranceCompany(insuranceCompany);
        setDate(date);
        this->nameFactory = nameFactory;
        this->performance = performance;
    }

    string getNameFactory() { return nameFactory; }
    int getPerformance() { return performance; }

    void setNameFactory(string nameFactory) { this->nameFactory = nameFactory; }
    void setPerformance(int performance) { this->performance = performance; }

    void show()
    {
        showOrganization();
        showInsuranceCompany();
        showDate();
        cout << "Завод: " << getNameFactory() << endl;
        cout << "Производительность завода: " << getPerformance() << endl << endl;
    }
};

class Person {
private:
    string name;
    int age;
public:
    void setName(string name) { this->name = name; }
    void setAge(int age) { this->age = age; }
    string getName() {return name; }
    int getAge() { return age; }
};


class Student : public Person {
private:
    int kurs;
public:
    Student(string name, int age, int kurs) {
        setName(name);
        setAge(age);
        this->kurs = kurs;
    }
    void setKurs(int kurs) {
        this->kurs = kurs;
    }

    int getKurs() { return kurs; }

    void showStudent() {
        cout << "Имя: " << getName() << endl;
        cout << "Возраст: " << getAge() << endl;
        cout << "Курс: " << kurs << endl;
    }
};

class Teacher : public Person {
private:
    string lesson;
public:
    Teacher(string name, int age, string lesson) {
        setName(name);
        setAge(age);
        this->lesson = lesson;
    }
    void setLesson(int lesson) {
        this->lesson = lesson;
    }

    string getLesson() { return lesson; }

    void showLesson() {
        cout << "Имя: " << getName() << endl;
        cout << "Возраст: " << getAge() << endl;
        cout << "Что ведет: " << lesson << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    string organization;
    string insuranceCompany;
    string date;
    string nameFactory;
    int performance;
    vector<Factory> v1;

    string name;
    int age;
    int kurs;
    string lesson;


    for (size_t i = 0; i < 2; i++)
    {
        cout << "Введите организацию: "; cin >> organization;
        cout << "Страховую компанию: ";    cin >> insuranceCompany;
        cout << "Введите дату создания: "; cin >> date;
        cout << "Название завода: ";    cin >> nameFactory;
        cout << "Производительность завода: ";    cin >> performance;
        cout << endl;

        Factory factory{ organization, insuranceCompany, nameFactory, performance, date };
        v1.push_back(factory);
    }

    for (size_t i = 0; i < v1.size(); i++)
    {
        v1[i].show();
    }

    cout << "Введите имя: "; cin >> name;
    cout << "Введите возраст: ";    cin >> age;
    cout << "Введите курс: "; cin >> kurs;

    Student student = { name, age, kurs };
    cout << endl;
    student.showStudent();

    cout << "Введите имя: "; cin >> name;
    cout << "Введите возраст: ";    cin >> age;
    cout << "Что ведёт: "; cin >> lesson;

    Teacher teacher = { name, age, lesson };
    cout << endl;
    teacher.showLesson();
}