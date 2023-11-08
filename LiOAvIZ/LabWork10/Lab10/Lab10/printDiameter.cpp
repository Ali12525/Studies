#include "Header.h"

void printDiameter(int** matrix, int size)
{
	if (getDiameter(matrix, size) == MAX)
		printf("\nDiameter: -\n");
	else
		printf("\nDiameter: %d\n", getDiameter(matrix, size));
}