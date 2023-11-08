#include "Header.h"

void printCentralVertex(int** matrix, int size)
{
	int min = getRadius(matrix, size);
	int* arreyEccentricity = getEccentricityArrey(matrix, size);
	printf("\nCentral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (min == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}