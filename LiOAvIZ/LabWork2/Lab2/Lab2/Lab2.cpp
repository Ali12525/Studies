#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10000

int main(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	clock_t start, end, startCycle;
	start = clock();

	int i = 0, j = 0, r;
	int elem_c;
	int** a,** b,** c;
	
	a = (int**)malloc(SIZE * sizeof(int*));

	for (int i = 0; i < SIZE; ++i)
	{
		a[i] = (int*)malloc(SIZE * sizeof(int));
	}

	b = (int**)malloc(SIZE * sizeof(int*));

	for (int i = 0; i < SIZE; ++i)
	{
		b[i] = (int*)malloc(SIZE * sizeof(int));
	}

	c = (int**)malloc(SIZE * sizeof(int*));

	for (int i = 0; i < SIZE; ++i)
	{
		c[i] = (int*)malloc(SIZE * sizeof(int));
	}

	srand(time(NULL));
	while (i < SIZE)
	{
		while (j < SIZE)
		{
			a[i][j] = rand() % 100 + 1;
			j++;
		}
		i++;
	}
	srand(time(NULL));
	i = 0; j = 0;
	while (i < SIZE)
	{
		while (j < SIZE)
		{
			b[i][j] = rand() % 100 + 1;
			j++;
		}
		i++;
	}

	startCycle = clock();

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			elem_c = 0;
			for (r = 0; r < SIZE; r++)
			{
				elem_c = elem_c + a[i][r] * b[r][j];
				c[i][j] = elem_c;
			}
		}
	}

	end = clock();

	printf("Size: %d", SIZE);
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\ntime program: %f c.", time_spent);
	time_spent = (double)(end - startCycle) / CLOCKS_PER_SEC;
	printf("\ntime matrix: %f c.", time_spent);

	return(0);
}
