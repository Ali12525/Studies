#ifndef FILE_WORK
#define FILE_WORK
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using std::ifstream;
using std::stoi;
using std::getline;

void writeMatrixInFile(int** array, int total);
int** readMatrixFromFile(string name, int total, int sizeInStr);
#endif
