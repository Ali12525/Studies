#include "Header.h"

using namespace std;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int sizeMatrix, start = 0, end = 0;
	int** matrix;
	int vertex = 0;
	int weighted = 1;
	int directed = 1;

	if (argc > 1)
	{
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp(argv[i], "-weighted") == 0)
				weighted = std::stoi(argv[i + 1]);

			if (strcmp(argv[i], "-directed") == 0)
				directed = std::stoi(argv[i + 1]);
		}
	}
	else
	{
		printf("Enter weighted: ");
		scanf("%d", &weighted);
		getchar();
		printf("Enter directed: ");
		scanf("%d", &directed);
		getchar();
	}

	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &sizeMatrix);
	int* ancestor = (int*)calloc(sizeMatrix, sizeMatrix * sizeof(int));
	int* arreyDistance = (int*)calloc(sizeMatrix, sizeMatrix * sizeof(int));
	arreyDistance = fillArreyDistance(arreyDistance, sizeMatrix);
	matrix = (int**)malloc(sizeMatrix * sizeof(int*));

	for (int i = 0; i < sizeMatrix; ++i)
	{
		matrix[i] = (int*)malloc(sizeMatrix * sizeof(int));
	}
	
	if(directed == 0)
		printf("\nFor undirected graph\n");
	else
		printf("\nFor directed graph\n");
	printf("\nAdjacency matrix\n");
	fillMatrix(matrix, sizeMatrix, directed, weighted);
	printMatrix(matrix, sizeMatrix);
	shortestPath(arreyDistance, ancestor, matrix, sizeMatrix, vertex);
	printShortestPath(arreyDistance, sizeMatrix, ancestor, vertex);
	printRadius(matrix, sizeMatrix);
	printDiameter(matrix, sizeMatrix);
	peripheralVertex(matrix, sizeMatrix);
	centralVertex(matrix, sizeMatrix);
	printf("\n");

	for (int i = 0; i < sizeMatrix; i++)
		free(matrix[i]);
	free(matrix);
	free(arreyDistance);
	free(ancestor);

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

int diameter(int** matrix, int size)
{
	int max = 0;
	int* arreyEccentricity = getEccentricityArrey(matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (max < arreyEccentricity[i])
			max = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return max;
}

int radius(int** matrix, int size)
{
	int min = MAX;
	int* arreyEccentricity = getEccentricityArrey(matrix, size);

	for (int i = 0; i < size; i++)
	{
		if (min > arreyEccentricity[i])
			min = arreyEccentricity[i];
	}

	free(arreyEccentricity);

	return min;
}

void printDiameter(int** matrix, int size)
{
	if(diameter(matrix, size) == MAX)
		printf("\nDiameter: -\n");
	else
		printf("\nDiameter: %d\n", diameter(matrix, size));
}

void printRadius(int** matrix, int size)
{
	if (radius(matrix, size) == MAX)
		printf("\nRadius: -\n");
	else
		printf("\nRadius: %d\n", radius(matrix, size));
}

int* getEccentricityArrey(int** matrix, int size)
{
	int* arreyEccentricity = (int*)calloc(size, size * sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int* arreyDistance = (int*)calloc(size, size * sizeof(int));
		arreyDistance = fillArreyDistance(arreyDistance, size);
		int* ancestor = (int*)calloc(size, size * sizeof(int));
		shortestPath(arreyDistance, ancestor, matrix, size, i);
		for (int j = 0; j < size; j++)
		{
			if (arreyEccentricity[i] < arreyDistance[j])
				arreyEccentricity[i] = arreyDistance[j];
		}
		free(arreyDistance);
		free(ancestor);
	}
	
	return arreyEccentricity;
}

void peripheralVertex(int** matrix, int size)
{
	int max = diameter(matrix, size);
	int* arreyEccentricity = getEccentricityArrey(matrix, size);
	printf("\nPeripheral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (max == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}

void centralVertex(int** matrix, int size)
{
	int min = radius(matrix, size);
	int* arreyEccentricity = getEccentricityArrey(matrix, size);
	printf("\nCentral vertex: ");

	for (int i = 0; i < size; i++)
	{
		if (min == arreyEccentricity[i])
			printf("%d ", i);
	}

	free(arreyEccentricity);
}

void shortestPath(int* arreyDistance, int* ancestor, int** matrix, int size, int vertex)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	std::priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
	arreyDistance[vertex] = 0;
	ancestor[vertex] = vertex;
	queue.push({ 0, vertex });

	while (!queue.empty())
	{
		pair<int, int> c = queue.top();
		queue.pop();
		vertex = c.second;
		int weight = c.first;
		if (weight > arreyDistance[vertex])
			continue;
		for (int i = 0; i < size; i++)
		{
			if (matrix[vertex][i] != MAX)
			{
				int v = i;
				int len = matrix[vertex][i];
				if (arreyDistance[v] > arreyDistance[vertex] + len) {
					ancestor[v] = vertex;
					arreyDistance[v] = arreyDistance[vertex] + len;
					queue.push({ arreyDistance[v], v });
				}
			}
		}
	}
	
	free(visited);
}

void printWay(int* arr, int size, int vertex)
{
	if (arr[vertex] != vertex)
		printWay(arr, size, arr[vertex]);
	printf("%d ", vertex);
}

void printMatrix(int** matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if(matrix[i][j] == MAX)
				printf("%3c ",'-');
			else
				printf("%3d ", matrix[i][j]);
		}
		printf("\n");
	}
}

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

void printShortestPath(int* arreyDistance, int size, int* ancestor, int vertex)
{
	for (int i = 0; i < size; i++)
	{
		if(arreyDistance[i] == MAX)
			printf("\nWeight: %d -> %d = -   ", vertex, i, arreyDistance[i]);
		else
			printf("\nWeight: %d -> %d = %d   ", vertex, i, arreyDistance[i]);
		printf("Way: ");
		printWay(ancestor, size, i);
	}
	printf("\n");
}

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
				if(directed == 0)
					matrix[j][i] = matrix[i][j];
			}
			else
			{
				matrix[i][j] = MAX;
				if(directed == 0)
					matrix[j][i] = matrix[i][j];
			}
		}
	}
}

int* fillArreyDistance(int* arreyDistance, int size)
{
	for (int i = 0; i < size; i++)
	{
		arreyDistance[i] = MAX;
	}

	return arreyDistance;
}