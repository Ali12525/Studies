#include "Header.h"

void printShortestPath(int* arreyDistance, int size, int* ancestor, int vertex)
{
	for (int i = 0; i < size; i++)
	{
		if (arreyDistance[i] == MAX)
			printf("\nWeight: %d -> %d = -   ", vertex, i, arreyDistance[i]);
		else
			printf("\nWeight: %d -> %d = %d   ", vertex, i, arreyDistance[i]);
		printf("Way: ");
		printWay(ancestor, size, i);
	}
	printf("\n");
}