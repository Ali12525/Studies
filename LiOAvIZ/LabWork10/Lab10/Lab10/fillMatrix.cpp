#include "Header.h"

void fillMatrix(int** matrix, int size, int directed, int weighted)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
			{
				matrix[i][j] = 0;
				continue;
			}
			if (rand() % 2 == 1)
			{
				if (weighted == 1)
					matrix[i][j] = rand() % 20 + 1;
				else
					matrix[i][j] = 1;
				if (directed == 0)
					matrix[j][i] = matrix[i][j];
			}
			else
			{
				matrix[i][j] = MAX;
				if (directed == 0)
					matrix[j][i] = matrix[i][j];
			}
		}
	}
}