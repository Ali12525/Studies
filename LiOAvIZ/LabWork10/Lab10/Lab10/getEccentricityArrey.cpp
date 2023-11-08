#include "Header.h"

int* getEccentricityArrey(int** matrix, int size)
{
	int* arreyEccentricity = (int*)calloc(size, size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int* arreyDistance = (int*)calloc(size, size * sizeof(int));
		arreyDistance = fillArreyDistance(arreyDistance, size);
		int* ancestor = (int*)calloc(size, size * sizeof(int));
		dijkstraAlgorithm(arreyDistance, ancestor, matrix, size, i);
		for (int j = 0; j < size; j++)
		{
			if (arreyEccentricity[i] < arreyDistance[j])
				arreyEccentricity[i] = arreyDistance[j];
		}
		free(arreyDistance);
		free(ancestor);
	}

	return arreyEccentricity;
}