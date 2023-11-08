#include "Header.h"

void printMatrix(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (matrix[i][j] == MAX)
				printf("%3c ", '-');
			else
				printf("%3d ", matrix[i][j]);
		}
		printf("\n");
	}
}