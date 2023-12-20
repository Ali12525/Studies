#include "QsortRecursive.h"

void qsortRecursive(int** mas, int size, int row, bool up)
{   
	int i = 0;
	int j = size - 1;

	int mid = mas[size / 2][row];
	do 
	{ 
		if (!up)
		{
			while (mas[i][row] > mid) 
				i++;

			while (mas[j][row] < mid) 
				j--;
			
		}
		else
		{
			while (mas[i][row] < mid) 
				i++;
			
			while (mas[j][row] > mid) 
				j--;
		}

		if (i <= j) 
		{
			int* tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;

			i++;
			j--;
		}
	} 
	while (i <= j);

	if (j > 0) 
		qsortRecursive(mas, j + 1, row, up);
	
	if (i < size) 
		qsortRecursive(&mas[i], size - i, row, up);
	
}