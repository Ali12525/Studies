#include "Header.h"

void printPeripheralVertex(int** matrix, int size)
{
	int max = getDiameter(matrix, size);
	int* arreyEccentricity = getEccentricityArrey(matrix, size);
	printf("\nPeripheral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (max == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}