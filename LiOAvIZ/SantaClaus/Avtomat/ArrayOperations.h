#ifndef ARRAY_OPERATIONS
#define ARRAY_OPERATIONS

#include <iostream>
#include <algorithm>

using std::cout;
using std::cin;

void Print3dArray(int*** array, int x, int y);
void Print2dArray(int** children, int x, int y);
int** Make2dArray(int x, int y, int z);
int*** Make3dArrayFrom2d(int** from, int x, int y, int coef);

#endif
