#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_arr(int** Arr, int size);
int* reset_arr(int* Peaks, int size);
void print_arr(int** Arr, int size_column, int size_row);
int size_arr(int** Arr, int size_column, int size_row);
int* peaks(int** Arr, int* Peaks, int Adjacency, int size_column, int size_row);
void fill_arr_inccidence(int** Arr, int** Arrey, int size);

int main(void)
{
	srand(time(NULL));
	int size;
	int** Arr;
	int* Peaks = (int*)malloc(3 * sizeof(int));

	reset_arr(Peaks, 3);
	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &size);
	Arr = (int**)malloc(size * sizeof(int*));

	for (int i = 0; i < size; ++i)
	{
		Arr[i] = (int*)malloc(size * sizeof(int));
	}

	printf("Adjacency matrix\n");
	fill_arr(Arr, size);
	print_arr(Arr, size, size);
	printf("size arr: %d\n", size_arr(Arr, size, size));
	Peaks = peaks(Arr, Peaks, 0, size, size);
	printf("isolated peaks: %d\n", Peaks[0]);
	printf("terminal peaks: %d\n", Peaks[1]);
	printf("dominant peaks: %d\n", Peaks[2]);
	reset_arr(Peaks, 3);

	printf("\n\nInccidence matrix\n");
	int** Arrey = (int**)malloc(size * sizeof(int*));

	for (int i = 0; i < size; ++i)
	{
		Arrey[i] = (int*)malloc(size_arr(Arr, size, size) * sizeof(int));
	}

	fill_arr_inccidence(Arr, Arrey, size);
	print_arr(Arrey, size, size_arr(Arr, size, size));
	printf("size arr: %d\n", size_arr(Arrey, size, size_arr(Arr, size, size)));
	Peaks = peaks(Arrey, Peaks, 1, size, size_arr(Arr, size, size));
	printf("isolated peaks: %d\n", Peaks[0]);
	printf("terminal peaks: %d\n", Peaks[1]);
	printf("dominant peaks: %d\n", Peaks[2]);

	free(Arr);
	free(Arrey);
	free(Peaks);

	return(0);
}

void print_arr(int** Arr, int size_column, int size_row)
{
	for (int i = 0; i < size_column; i++)
	{
		for (int j = 0; j < size_row; j++)
		{
			printf("%3d ", Arr[i][j]);
		}
		printf("\n");
	}
}

void fill_arr(int** Arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
			{
				Arr[i][j] = 0;
				continue;
			}
			Arr[i][j] = rand() % 2;
			Arr[j][i] = Arr[i][j];
		}
	}
}

void fill_arr_inccidence(int** Arr, int** Arrey, int size)
{
	int c = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size_arr(Arr, size, size); j++)
		{
			Arrey[i][j] = 0;
		}
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (Arr[i][j]) {
				Arrey[i][c] = 1;
				Arrey[j][c++] = 1;
			}
		}
	}
}

int size_arr(int** Arr, int size_column, int size_row)
{
	int count = 0;

	for (int i = 0; i < size_column; i++)
	{
		for (int j = 0; j < size_row; j++)
		{
			if(Arr[i][j] == 1)
				count++;
		}
	}

	return count / 2;
}

int* peaks(int** Arr, int* Peaks, int Adjacency, int size_column, int size_row)
{
	int count = 0;
	int peaks = 0;

	for (int i = 0; i < size_column; i++)
	{
		for (int j = 0; j < size_row; j++)
		{
			if (Arr[i][j] == 1)
				count++;
		}

		if (count == 0)
			Peaks[0]++;
		if (count == 1)
			Peaks[1]++;
		if (Adjacency == 0)
		{
			if (count == size_row - 1)
				Peaks[2]++;
		}
		else
		{
			if (count == size_column - 1)
				Peaks[2]++;
		}
		count = 0;
	}

	return Peaks;
}

int* reset_arr(int* Peaks, int size)
{
	for (int i = 0; i < size; i++)
	{
		Peaks[i] = 0;
	}

	return Peaks;
}