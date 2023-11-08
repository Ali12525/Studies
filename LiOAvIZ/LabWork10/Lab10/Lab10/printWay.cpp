#include "Header.h"

void printWay(int* arr, int size, int vertex)
{
	if (arr[vertex] != vertex)
		printWay(arr, size, arr[vertex]);
	printf("%d ", vertex);
}