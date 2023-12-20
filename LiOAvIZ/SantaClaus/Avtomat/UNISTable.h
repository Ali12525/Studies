#ifndef UNIS_TABLE
#define UNIS_TABLE

#include <iostream>
#include <map>
#include <vector>

using std::cout;
using std::cin;
using std::vector;
using std::map;

int FindChildrenInPresent(int*** presents, int presentNum, int children, map<int, int> allPresents);
int FindPresentInChildren(int*** children, int childNum, int present, map<int, int> allPresents);

int** UNISTable(int*** children, int*** presents, int* unis);

#endif

