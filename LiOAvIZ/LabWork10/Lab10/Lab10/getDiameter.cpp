#include "Header.h"

int getDiameter(int** matrix, int size)
{
	int max = 0;
	int* arreyEccentricity = getEccentricityArrey(matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (max < arreyEccentricity[i])
			max = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return max;
}