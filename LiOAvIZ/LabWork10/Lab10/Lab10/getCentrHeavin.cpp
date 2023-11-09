#include "Header.h"

pair<int, int> getCentrHeavin(int** matrix, int size)
{
	int min = 0;
	int vertexHeavin = 0;
	int* Arr = (int*)calloc(size, size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int vertex = i;
		int* ancestor = (int*)calloc(size, size * sizeof(int));
		int* arreyDistance = (int*)calloc(size, size * sizeof(int));
		arreyDistance = fillArreyDistance(arreyDistance, size);
		dijkstraAlgorithm(arreyDistance, ancestor, matrix, size, vertex);
		Arr[i] = sum(arreyDistance, size);
		free(arreyDistance);
		free(ancestor);
	}

	min = getMin(Arr, size);

	printf("\n");
	printArr(Arr, size);
	printf("\n");

	for (int i = 0; i < size; i++)
	{
		if (min == Arr[i])
			vertexHeavin = i;
	}

	return pair<int, int>(min, vertexHeavin);
}

int sum(int* arreyDistance, int size)
{
	int countSumm = 0;

	for (int i = 0; i < size; i++)
	{
		countSumm += arreyDistance[i];
	}

	return countSumm;
}

int getMin(int* Arr, int size)
{
	int min = MAX;

	for (int i = 0; i < size; i++)
	{
		if (min > Arr[i])
			min = Arr[i];
	}

	return min;
}