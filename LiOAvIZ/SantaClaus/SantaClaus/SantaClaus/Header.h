#pragma once

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>

using std::cerr;
using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::stringstream;
using std::vector;

#define CHILD_COUNT 184000
#define CHILD_PRESENTS 100

#define CHILD_TWEENS 5520 

#define PRESENTS_COUNT 1000
#define PRESENTS_CHILD 184

int** readMatrixFromFile(std::string name, int total, int sizeInStr);
int** Make2dArray(int x, int y, int z);
void Print2dArray(int** children, int x, int y);
int*** Make3dArrayFrom2d(int** from, int x, int y);
void Print3dArray(int*** array, int x, int y);
int FindChildrenInPresent(int*** presents, int presentNum, int children, std::map<int, int> allPresents);
int FindPresentInChildren(int*** children, int childNum, int present, std::map<int, int> allPresents);
void qsortRecursive(int** mas, int size, int row, bool up);
int** UNISTable(int*** children, int*** presents, int* unis);
void writeMatrixInFile(int** array, int total);