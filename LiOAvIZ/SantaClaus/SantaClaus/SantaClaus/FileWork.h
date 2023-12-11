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

int** readMatrixFromFile(std::string name, int total, int sizeInStr);
void writeMatrixInFile(int** array, int total);