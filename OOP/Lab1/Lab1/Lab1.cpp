#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
using namespace std;

class Train {
private:
    string destination;
    int number; 
    int time;
    int mainCount; 
    int kupeCount;
    int plackartCount;
public:
    Train(string destination, int number, int time, int mainCount, int kupeCount, int plackartCount)
    {
        this->destination = destination;
        this->number = number;
        this->time = time;
        this->mainCount = mainCount;
        this->kupeCount = kupeCount;
        this->plackartCount = plackartCount;
    }

    Train(const Train& t)
    {
        this->destination = t.destination;
        this->number = t.number;
        this->time = t.time;
        this->mainCount = t.mainCount;
        this->kupeCount = t.kupeCount;
        this->plackartCount = t.plackartCount;
    }

    ~Train()
    {
        
    }

    void show() {
        cout << endl;
        cout << "Пункт назначения: " << destination << endl;
        cout << "Номер: " << number << endl;
        cout << "Время: " << time << endl;
        cout << "Общее количество мест: " << mainCount << endl;
        cout << "Мест в купе: " << kupeCount << endl;
        cout << "Мест в плацкарте: " << plackartCount << endl;

    }

    string getDestination() { return destination; }
    int getTime() { return time; }
    int getMainCount() { return mainCount; }
    int getNumber() { return number; }
    int getKupeCount() { return kupeCount; }
    int getPupeCount() { return plackartCount; }


    string setDestination(string destination) { 
        this->destination = destination;
    }
    int setTime(int time) { 
        this->time = time;
    }
    int setMainCount(int mainCount) { 
        this->mainCount = mainCount;
    }
    int setNumber(int number) { 
        this->number = number;
    }
    int setKupeCount(int kupeCount) { 
        this->kupeCount = kupeCount;
    }
    int setPupeCount(int plackartCount) { 
        this->plackartCount = plackartCount;
    }
};

class Time {
private:
    int hours;
    int minutes;
    int seconds;
public:
    Time(int hour, int minute, int second) {
        this->hours = hour;
        this->minutes = minute;
        this->seconds = second;
    }

    Time(const Time& t)
    {
        this->hours = t.hours;
        this->minutes = t.minutes;
        this->seconds = t.seconds;
    }

    ~Time()
    {

    }

    int getHour() { return this->hours; }
    int getMinute() { return this->minutes; }
    int getSecond() { return this->seconds; }


    int setHour(int hour) {
        this->hours = hour;
    }

    int setMinute(int minute) {
        this->minutes = minute;
    }

    int setSecond(int second) {
        this->seconds = second;
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    string destination;
    int number;
    int time;
    int mainCount;
    int kupeCount;
    int plackartCount;

    vector<Train> v1; // массив поездов

    for (size_t i = 0; i < 5; i++)
    {
        cout << "Введите данные о поезде " << i + 1<< endl;
        cout << "Пункт назначения: ";     cin >> destination;
        cout << "Номер: ";    cin >> number;
        cout << "Время: ";  cin >> time;
        cout << "Общее количество мест: ";    cin >> mainCount;
        cout << "Мест в купе: ";  cin >> kupeCount;
        cout << "Мест в плацкарте: ";  cin >> plackartCount;

        cout << endl;

        Train t{ destination, number, time, mainCount, kupeCount, plackartCount };
        v1.push_back(t);
    }

    Train t(v1[0]);

    v1.push_back(t);


     cout << "Введите искомый пункт назначения: "; cin >> destination;
     cout << "Введите время: "; cin >> time;
     cout << "Введите количество мест: "; cin >> mainCount;
     
     cout << "Поезда, следующие до заданного пункта назначения:" << endl;

     for (size_t i = 0; i < v1.size(); i++)
     {
         if (v1[i].getDestination() == destination)
         {
             cout << v1[i].getNumber() << endl;
         }
     }

     cout << endl;
     cout << "Поезда, следующие до заданного пункта назначения с учетом времени:" << endl;

     for (size_t i = 0; i < v1.size(); i++)
     {
         if (v1[i].getDestination() == destination && v1[i].getTime() > time)
         {
             cout << v1[i].getNumber() << endl;
         }
     }

     cout << endl;
     cout << "Поезда, следующие до заданного пункта назначения с учетом мест:" << endl;

     for (size_t i = 0; i < v1.size(); i++)
     {
         if (v1[i].getDestination() == destination && v1[i].getMainCount() == mainCount)
         {
             cout << v1[i].getNumber() << endl;
         }
     }
     
     cout << "Размер массива: " << v1.size();
}