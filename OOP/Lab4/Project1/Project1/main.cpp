#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

using namespace std;

template <class T = int>
class MySet
{
private:
    int size = 0;
    T* Arr = NULL;
public:

    MySet(T* p)
    {
        Arr = p;
    }

    bool check(T item)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (Arr[i] == item)
                return false;
        }

        return true;
    }

    void insert(T item)
    {
        if (size == 0)
        {
            size = 1;
            Arr = new T[size];
            Arr[0] = item;
        }
        else
        {
            if (check(item) == true)
            {
                size++;
                T* Arrey = new T[size];

                for (size_t i = 0; i < size - 1; i++)
                {
                    Arrey[i] = Arr[i];
                }

                Arrey[size - 1] = item;

                delete[] Arr;
                Arr = Arrey;
            }
        }
    }

    void erase(T item)
    {
        int flag = 0;
        size--;
        T* Arrey = new T[size];

        for (size_t i = 0; i < size; i++)
        {
            if (Arr[i] == item || flag == 1)
            {
                Arrey[i] = Arr[i + 1];
                flag = 1;
            }
            else
                Arrey[i] = Arr[i];
        }

        delete[] Arr;
        Arr = Arrey;
    }

    int getSize() { return size; }

    bool count(T item) {
        for (int i = 0; i < size; i++)
        {
            if (Arr[i] == item)
            {
                return true;
            }
        }
        return false;
    }

    MySet& operator * (MySet& s)
    {
        T* Arr3 = NULL;
        MySet copy{ Arr3 };

        for (int i = 0; i < size; i++) {
            for (int c = 0; c < s.getSize(); c++) {
                if (Arr[i] == s.Arr[c] && copy.count(Arr[i]) == false) {
                    copy.insert(Arr[i]);
                }
            }
        }

        return copy;

    }

    MySet& operator- (T s)
    {
        erase(s);
        return *this;
    }

    bool operator < (MySet& s)
    {
        if (size < s.getSize())
            return true;
        return false;
    }

    void show() {
        for (int i = 0; i < size; i++)
        {
            cout << Arr[i] << " ";
        }
        cout << endl;
    };
};

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int* p1 = NULL;
    int* p2 = NULL;

    MySet<int> arr1{ p1 };
    MySet<int> arr2{ p2 };

    for (int i = 0; i < 5; i++) {
        arr1.insert(i);
    };

    for (int i = 0; i < 6; i++) {
        arr2.insert(i);
    }

    cout << "Множество 1:" << endl;
    arr1.show();
    cout << "Множество 2:" << endl;
    arr2.show();

    cout << "Множество 1 - '2':" << endl;
    arr1 = arr1 - (2);
    arr1.show();
    cout << "Множество 2 - '4':" << endl;
    arr2 = arr2 - (4);
    arr2.show();

    if (arr1 < arr2)
        cout << "Текущее множество меньше" << endl;
    else if (arr1.getSize() == arr2.getSize())
        cout << "Множества одинаковы" << endl;
    else
        cout << "Текущее множество больше" << endl;

    cout << "Множество 1 * 2:" << endl;
    MySet<int> arr3{ arr1 * arr2 };
    arr3.show();
}