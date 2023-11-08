#include "Header.h"

void printRadius(int** matrix, int size)
{
	if (getRadius(matrix, size) == MAX)
		printf("\nRadius: -\n");
	else
		printf("\nRadius: %d\n", getRadius(matrix, size));
}