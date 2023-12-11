#ifndef ARRAY_OP
#define ARRAY_OP
#include "Present.h"


Present** CreateEmptyArray(int size);
void AddWeightChild(Present** a, int** arrChild);
void AddWeightPresent(Present** a, int** arrPresent);

#endif