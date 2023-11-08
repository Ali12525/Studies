#include "Header.h"

void printArr(int* arreyDistance, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (arreyDistance[i] == MAX)
			printf("- ", arreyDistance[i]);
		else
			printf("%d ", arreyDistance[i]);
	}
}