#include "Header.h"
#include <queue>
#include <iostream>

using namespace std;

int main(void)
{
	srand(time(NULL));
	int sizeMatrix, start = 0, end = 0;
	int** Arr;
	int vertex = 0;
	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &sizeMatrix);
	int* predok = (int*)calloc(sizeMatrix, sizeMatrix * sizeof(int));
	int* ArreyDistance = (int*)calloc(sizeMatrix, sizeMatrix * sizeof(int));
	ArreyDistance = fillArreyDistance(ArreyDistance, sizeMatrix);
	Arr = (int**)malloc(sizeMatrix * sizeof(int*));

	for (int i = 0; i < sizeMatrix; ++i)
	{
		Arr[i] = (int*)malloc(sizeMatrix * sizeof(int));
	}
	
	printf("\nFor undirected graph\n");
	printf("\nAdjacency matrix\n");
	fill_arr(Arr, sizeMatrix, 0);
	print_arr(Arr, sizeMatrix);
	shortestPath(ArreyDistance, predok, Arr, sizeMatrix, vertex);
	printShortestPath(ArreyDistance, sizeMatrix, predok, vertex);
	printRadius(Arr, sizeMatrix);
	printDiametr(Arr, sizeMatrix);
	peripheralVertex(Arr, sizeMatrix);
	centralVertex(Arr, sizeMatrix);

	ArreyDistance = fillArreyDistance(ArreyDistance, sizeMatrix);
	free(predok);
	predok = (int*)calloc(sizeMatrix, sizeMatrix * sizeof(int));
	printf("\n\n\n\nFor directed graph\n");
	printf("\nAdjacency matrix\n");
	fill_arr(Arr, sizeMatrix, 1);
	print_arr(Arr, sizeMatrix);
	shortestPath(ArreyDistance, predok, Arr, sizeMatrix, vertex);
	printShortestPath(ArreyDistance, sizeMatrix, predok, vertex);
	printRadius(Arr, sizeMatrix);
	printDiametr(Arr, sizeMatrix);
	peripheralVertex(Arr, sizeMatrix);
	centralVertex(Arr, sizeMatrix);

	for (int i = 0; i < sizeMatrix; i++)
		free(Arr[i]);
	free(Arr);
	free(ArreyDistance);
	free(predok);

	return 0;
}

struct node
{
	int vertex;
	struct node* next;
};

struct Graph
{
	int numVertices;
	struct node** adjLists;
};

int diametr(int** Matrix, int size)
{
	int max = 0;
	int* arreyEccentricity = getEccentricityArrey(Matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (max < arreyEccentricity[i])
			max = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return max;
}

int radius(int** Matrix, int size)
{
	int min = MAX;
	int* arreyEccentricity = getEccentricityArrey(Matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (min > arreyEccentricity[i])
			min = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return min;
}

void printDiametr(int** Matrix, int size)
{
	if(diametr(Matrix, size) == MAX)
		printf("\nDiametr: -\n");
	else
		printf("\nDiametr: %d\n", diametr(Matrix, size));
}

void printRadius(int** Matrix, int size)
{
	if (radius(Matrix, size) == MAX)
		printf("\nRadius: -\n");
	else
		printf("\nRadius: %d\n", radius(Matrix, size));
}

int* getEccentricityArrey(int** Matrix, int size)
{
	int* arreyEccentricity = (int*)calloc(size, size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int* ArreyDistance = (int*)calloc(size, size * sizeof(int));
		ArreyDistance = fillArreyDistance(ArreyDistance, size);
		int* predok = (int*)calloc(size, size * sizeof(int));
		shortestPath(ArreyDistance, predok, Matrix, size, i);
		for (int j = 0; j < size; j++)
		{
			if (arreyEccentricity[i] < ArreyDistance[j])
				arreyEccentricity[i] = ArreyDistance[j];
		}
		free(ArreyDistance);
		free(predok);
	}
	
	return arreyEccentricity;
}

void peripheralVertex(int** Matrix, int size)
{
	int max = diametr(Matrix, size);
	int* arreyEccentricity = getEccentricityArrey(Matrix, size);
	printf("\nPeripheral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (max == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}

void centralVertex(int** Matrix, int size)
{
	int min = radius(Matrix, size);
	int* arreyEccentricity = getEccentricityArrey(Matrix, size);
	printf("\nCentral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (min == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}

void shortestPath(int* ArreyDistance, int* predok, int** Matrix, int size, int vertex)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	std::priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
	ArreyDistance[vertex] = 0;
	predok[vertex] = vertex;
	queue.push({ 0, vertex });

	while (!queue.empty())
	{
		pair<int, int> c = queue.top();
		queue.pop();
		vertex = c.second;
		int weight = c.first;
		if (weight > ArreyDistance[vertex])
			continue;
		for (int i = 0; i < size; i++)
		{
			if (Matrix[vertex][i] != MAX)
			{
				int v = i;
				int len = Matrix[vertex][i];
				if (ArreyDistance[v] > ArreyDistance[vertex] + len) {
					predok[v] = vertex;
					ArreyDistance[v] = ArreyDistance[vertex] + len;
					queue.push({ ArreyDistance[v], v });
				}
			}
		}
	}
	
	free(visited);
}

void printWay(int* Arr, int size, int vertex)
{
	if (Arr[vertex] != vertex)
		printWay(Arr, size, Arr[vertex]);
	printf("%d ", vertex);
}

void print_arr(int** Arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if(Arr[i][j] == MAX)
				printf("%3c ",'-');
			else
				printf("%3d ", Arr[i][j]);
		}
		printf("\n");
	}
}

void printArr(int* ArreyDistance, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (ArreyDistance[i] == MAX)
			printf("- ", ArreyDistance[i]);
		else
			printf("%d ", ArreyDistance[i]);
	}
}

void printShortestPath(int* ArreyDistance, int size, int* predok, int vertex)
{
	for (int i = 0; i < size; i++)
	{
		if(ArreyDistance[i] == MAX)
			printf("\nWeight: %d -> %d = -   ", vertex, i, ArreyDistance[i]);
		else
			printf("\nWeight: %d -> %d = %d   ", vertex, i, ArreyDistance[i]);
		printf("Way: ");
		printWay(predok, size, i);
	}
	printf("\n");
}

void fill_arr(int** Arr, int size, int oriented)
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
			if (rand() % 2 == 1)
			{
				Arr[i][j] = rand() % 20 + 1;
				if(oriented == 0)
					Arr[j][i] = Arr[i][j];
			}
			else
			{
				Arr[i][j] = MAX;
				if(oriented == 0)
					Arr[j][i] = MAX;
			}
		}
	}
	/*Arr[0][0] = MAX;    Arr[0][1] = 4;    Arr[0][2] = MAX;    Arr[0][3] = 1;    Arr[0][4] = 3; Arr[0][5] = MAX;
	Arr[1][0] = MAX;     Arr[1][1] = MAX;    Arr[1][2] = 3;     Arr[1][3] = MAX;   Arr[1][4] = MAX; Arr[1][5] = MAX;
	Arr[2][0] = MAX;   Arr[2][1] = MAX;    Arr[2][2] = MAX;     Arr[2][3] = MAX;   Arr[2][4] = MAX; Arr[2][5] = MAX;
	Arr[3][0] = MAX;    Arr[3][1] = MAX;    Arr[3][2] = MAX;    Arr[3][3] = MAX; Arr[3][4] = 1; Arr[3][5] = 2;
	Arr[4][0] = MAX;    Arr[4][1] = MAX;    Arr[4][2] = 4;    Arr[4][3] = MAX; Arr[4][4] = MAX; Arr[4][5] = MAX;
	Arr[5][0] = MAX;    Arr[5][1] = MAX;    Arr[5][2] = 2;    Arr[5][3] = MAX; Arr[5][4] = MAX; Arr[5][5] = MAX;*/

}

int* fill_visited(int* visited, int size)
{
	for (int i = 0; i < size; i++)
	{
		visited[i] = -1;
	}

	return visited;
}

int* fillArreyDistance(int* ArreyDistance, int size)
{
	for (int i = 0; i < size; i++)
	{
		ArreyDistance[i] = MAX;
	}

	return ArreyDistance;
}