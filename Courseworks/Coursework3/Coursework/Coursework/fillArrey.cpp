#include "Header.h"

int* fillArrey(int* arr, int size, int x)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = x;
	}

	return arr;
}