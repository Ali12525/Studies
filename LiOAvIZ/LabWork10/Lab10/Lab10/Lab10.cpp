#include "Header.h"
#include <queue>
#include <iostream>

using namespace std;

int main(void)
{
	srand(time(NULL));
	int sizeMatrix, start = 0, end = 0;
	int** Arr;

	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &sizeMatrix);

	Arr = (int**)malloc(sizeMatrix * sizeof(int*));

	for (int i = 0; i < sizeMatrix; ++i)
	{
		Arr[i] = (int*)malloc(sizeMatrix * sizeof(int));
	}

	printf("\nAdjacency matrix\n");
	fill_arr(Arr, sizeMatrix);
	print_arr(Arr, sizeMatrix);
	shortestPath(Arr, sizeMatrix, 0);

	for (int i = 0; i < sizeMatrix; i++)
		free(Arr[i]);
	free(Arr);

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

vector<pair<int, int>> graph[5];

void shortestPath(int** Matrix, int size, int vertex)
{
	int* ArreyDistance = (int*)calloc(size, size * sizeof(int));
	ArreyDistance = fillArreyDistance(ArreyDistance, size);
	int* visited = (int*)calloc(size, size * sizeof(int));

	std::priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
	ArreyDistance[vertex] = 0;
	queue.push({ vertex, 0});

	while (!queue.empty())
	{
		pair<int, int> c = queue.top();
		queue.pop();
		vertex = c.second;
		int weight = c.first;
		visited[vertex] = 1;
		if (ArreyDistance[vertex] < weight) {
			continue;
		}
		for (int i = 0; i < size; i++)
		{
			if (Matrix[vertex][i] != MAX && visited[i] == 0)
			{
				if (ArreyDistance[i] > Matrix[vertex][i] + weight)
				{
					ArreyDistance[i] = Matrix[vertex][i] + weight;
					queue.push({ Matrix[vertex][i], i });
				}
			}
		}
	}
	
	printArr(ArreyDistance, size);
}

int Bfs(int** Matrix, int size, int vertex, int end, int shutdown)
{
	std::queue<int> queue;
	queue.push(vertex);
	int* visited = (int*)calloc(size, size * sizeof(int));
	visited = fill_visited(visited, size);
	visited[vertex] = 0;
	if (!shutdown)
		printf("way: ");

	while (!queue.empty())
	{
		vertex = queue.front();
		queue.pop();
		if (!shutdown)
			printf("%d ", vertex);
		if (vertex == end)
			break;
		for (int i = 0; i < size; i++)
		{
			if (Matrix[vertex][i] == 1 && visited[i] == -1)
			{
				queue.push(i);
				visited[i] = visited[vertex] + 1;
			}
		}
	}

	int data = visited[end];
	if (size != 0)
		free(visited);
	return data;
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

void fill_arr(int** Arr, int size)
{
	/*for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
			{
				Arr[i][j] = 0;
				continue;
			}
			if (rand() % 3 == 1)
			{
				Arr[i][j] = rand() % 20;
				Arr[j][i] = Arr[i][j];
			}
			else
			{
				Arr[i][j] = MAX;
				Arr[j][i] = MAX;
			}
		}
	}*/
	Arr[0][0] = 0;    Arr[0][1] = 3;    Arr[0][2] = 1;    Arr[0][3] = 3;    Arr[0][4] = MAX; Arr[0][5] = MAX;
	Arr[1][0] = 3;     Arr[1][1] = 0;    Arr[1][2] = 4;     Arr[1][3] = MAX;   Arr[1][4] = MAX; Arr[1][5] = MAX;
	Arr[2][0] = 1;   Arr[2][1] = 4;    Arr[2][2] = 0;     Arr[2][3] = MAX;   Arr[2][4] = 7; Arr[2][5] = 5;
	Arr[3][0] = 3;    Arr[3][1] = MAX;    Arr[3][2] = MAX;    Arr[3][3] = 0; Arr[3][4] = MAX; Arr[3][5] = 2;
	Arr[4][0] = MAX;    Arr[4][1] = MAX;    Arr[4][2] = 7;    Arr[4][3] = MAX; Arr[4][4] = 0; Arr[4][5] = 4;
	Arr[5][0] = MAX;    Arr[5][1] = MAX;    Arr[5][2] = 5;    Arr[5][3] = 2; Arr[5][4] = 4; Arr[5][5] = 0;

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