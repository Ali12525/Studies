#include "Header.h"

int getRadius(int** matrix, int size)
{
	int min = MAX;
	int* arreyEccentricity = getEccentricityArrey(matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (min > arreyEccentricity[i])
			min = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return min;
}