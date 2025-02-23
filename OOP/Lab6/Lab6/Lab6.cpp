#include <iostream>
#include <windows.h>
#include <map>

using namespace std;

void printMap(map<int, int>& mp)
{
    map <int, int> ::iterator it = mp.begin();
    cout << "map:" << endl;
    for (int i = 0; it != mp.end(); it++, i++)
    {
        cout << i << ") Ключ: " << it->first << ", значение: " << it->second << endl << endl;
    }
}

void fillMap(map<int, int>& mp)
{
    cout << "Введите количество элементов: ";
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int k, m;
        cout << "Введите ключ для " << i << " элемента: ";
        cin >> k;
        cout << "Введите значение " << i << " элемента: ";
        cin >> m;
        mp.insert(make_pair(k, m));
    }
    cout << endl;
}

void eraseElementMap(map<int, int>& mp)
{
    int n, key;
    map <int, int> ::iterator it;
    cout << "Введите количество удаляемых элементов: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cout << "Введите ключ удаляемого элемента: ";
        cin >> key;

        it = mp.find(key);
        if (it != mp.end())
            mp.erase(it);
        else
            cout << "Вы ввели неправильный ключ!";
    }
}

void replacingАnЕlementMap(map<int, int>& mp)
{
    int n, key, meaning;
    map <int, int> ::iterator it;
    cout << "Введите количество заменяемых элементов: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cout << "Введите ключ заменяемого элемента: ";
        cin >> key;

        it = mp.find(key);
        if (it != mp.end())
        {
            mp.erase(it);
            cout << "Введите значение заменяемого элемента: ";
            cin >> meaning;
            mp.insert(make_pair(key, meaning));
        }
        else
            cout << "Вы ввели неправильный ключ!";
    }
}

void deletingItemsAfterTheKey(map<int, int>& mp)
{
    int n, key;
    map <int, int> ::iterator it;
    map <int, int> ::iterator it2;
    cout << "Введите после какого ключа удалять элементы: ";
    cin >> key;
    cout << "Введите сколько элементов удалить: ";
    cin >> n;
    cout << endl;
    it = mp.begin();
    int j = 0;

    while (it != mp.end())
    {
        if (it->first > key && j < n)
        {
            it2 = it;
            it++;
            mp.erase(it2);
            j++;
            continue;
        }
        it++;
    }
}

void addMap2ToMap1(map<int, int>& mp2, map<int, int>& mp)
{
    map <int, int> ::iterator it;
    it = mp2.begin();

    for (; it != mp2.end(); it++)
    {
        mp.insert(make_pair(it->first, it->second));
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    map <int, int> mp;
    map <int, int> mp2;

    cout << "Заполнение 1 контейнера" << endl;
    fillMap(mp);
    printMap(mp);

    cout << "Изменение 1 контейнера" << endl;
    eraseElementMap(mp);
    replacingАnЕlementMap(mp);
    printMap(mp);

    cout << "Заполнение 2 контейнера" << endl;
    fillMap(mp2);

    cout << "Изменение 1 контейнера" << endl;
    deletingItemsAfterTheKey(mp);
    addMap2ToMap1(mp2, mp);
    
    cout << "Map1:" << endl;
    printMap(mp);
    cout << "Map2:" << endl;
    printMap(mp2);
}