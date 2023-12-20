#include "ArrayOperations.h"

void Print3dArray(int*** array, int x, int y)
{
	for (size_t i = 0; i < x; i++)
	{
		std::cout << i << ": ";
		for (size_t j = 0; j < y; j++)
		{
			std::cout << "[" << array[i][j][0] << ", " << array[i][j][1] << "]";
		}
		std::cout << std::endl;
	}
}

void Print2dArray(int** children, int x, int y)
{
	for (size_t i = 0; i < x; i++)
	{
		std::cout << i << ": ";

		for (size_t j = 0; j < y; j++)
		{
			std::cout << children[i][j] << ", ";
		}

		std::cout << std::endl;
	}
}

int** Make2dArray(int x, int y, int z)
{
	int** array = new int* [x];
	for (size_t i = 0; i < x; i++)
	{
		array[i] = new int[y];

		int* tmp = new int[z];
		for (size_t j = 0; j < z; j++)
			tmp[j] = j;
		std::random_shuffle(tmp, tmp + z);

		for (size_t j = 0; j < y; j++)
		{
			array[i][j] = tmp[j];
		}
	}

	return array;
}

int*** Make3dArrayFrom2d(int** from, int x, int y, int coef)
{
	int*** weightArray = new int** [x];
	for (size_t i = 0; i < x; i++)
	{
		weightArray[i] = new int* [y];
		for (size_t j = 0; j < y; j++)
		{
			int unisPerChild = 2 * (coef - j);
			if (unisPerChild < 0)
				unisPerChild = -1;

			weightArray[i][j] = new int[2] { from[i][j], unisPerChild };
		}
	}

	return weightArray;
}