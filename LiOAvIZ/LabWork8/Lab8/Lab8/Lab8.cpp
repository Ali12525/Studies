#include "Header.h"
#include <queue>
#include <iostream>

using namespace std;

int main(void)
{
	srand(time(NULL));
	int sizeMatrix;
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
	struct Graph* graph = transformGraph(Arr, sizeMatrix);
	printGraph(graph);

	printf("\nBFS in the adjacency matrix");
	BFS(Arr, sizeMatrix, 0);

	printf("\n\nBFS in the adjacency list");
	BFSList(graph, sizeMatrix);

	printf("\n\nNo class BFS in the adjacency matrix");
	noClassBFS(Arr, sizeMatrix, 0);

	autoTest();

	free(Arr);

	return(0);
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

void BFS(int** Matrix, int size, int shutdown)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			if(!shutdown)
				printf("\nway: ");
			Bfs(Matrix, size, i, visited, shutdown);
		}
	}
}

void Bfs(int** Matrix, int size, int vertex, int* visited, int shutdown)
{
	std::queue<int> queue;
	queue.push(vertex);	
	visited[vertex] = 1;

	while (!queue.empty())
	{
		vertex = queue.front();
		queue.pop();
		if (!shutdown)
			printf("%d ", vertex);
		for (int i = 0; i < size; i++)
		{
			if (Matrix[vertex][i] == 1 && visited[i] == 0)
			{
				queue.push(i);
				visited[i] = 1;
			}
		}
	}

}

void BFSList(struct Graph* graph, int size)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			printf("\nway: ");
			BfsList(graph, i, visited);
		}
	}
}

void BfsList(struct Graph* graph, int vertex, int* visited)
{
	std::queue<int> queue;
	queue.push(vertex);

	while (!queue.empty())
	{
		vertex = queue.front();
		queue.pop();
		if (visited[vertex] == 1)
			continue;
		visited[vertex] = 1;
		printf("%d ", vertex);

		struct node* temp = graph->adjLists[vertex];
		while (temp)
		{
			int i = temp->vertex;
			if (visited[i] == 0)
				queue.push(i);
			temp = temp->next;
		}
	}
}

void noClassBFS(int** Matrix, int size, int shutdown)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			if (!shutdown)
				printf("\nway: ");
			noClassBfs(Matrix, size, i, visited, shutdown);
		}
	}
}

void noClassBfs(int** Matrix, int size, int vertex, int* visited, int shutdown)
{
	std::queue<int> queue;
	push(vertex);

	while (!queueEmpty())
	{
		vertex = pop();
		
		if (visited[vertex] == 1)
			continue;
		visited[vertex] = 1;
		if (!shutdown)
			printf("%d ", vertex);
		for (int i = 0; i < size; i++)
		{
			if (Matrix[vertex][i] == 1 && visited[i] == 0)
			{
				push(i);
			}
		}
	}

}

void print_arr(int** Arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
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
			if (rand() % 3 == 1)
			{
				Arr[i][j] = 1;
				Arr[j][i] = 1;
			}
			else
			{
				Arr[i][j] = 0;
				Arr[j][i] = 0;
			}
		}
	}
}

void autoTest()
{
	printf("\n\n              Auto test");
	printf("\n--------------------------------------\n");
	printf("| size |time with class|time no class|\n");
	for (int size = 0; size <= 5000; size += 500)
	{
		printf("|%6d|", size);
		int** Arr = (int**)malloc(size * sizeof(int*));
		for (int i = 0; i < size; ++i)
		{
			Arr[i] = (int*)malloc(size * sizeof(int));
		}
		fill_arr(Arr, size);

		clock_t start = clock();
		BFS(Arr, size, 1);
		clock_t end = clock();
		double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
		printf("%12f s.", time_spent);

		start = clock();
		noClassBFS(Arr, size, 1);
		end = clock();
		time_spent = (double)(end - start) / CLOCKS_PER_SEC;
		printf("|%10f s.|\n", time_spent);

		free(Arr);
	}
	printf("--------------------------------------\n");
}

struct node* createNode(int v)
{
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

struct Graph* createGraph(int vertices)
{
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->numVertices = vertices;

	graph->adjLists = (struct node**)malloc(vertices * sizeof(struct node*));

	for (int i = 0; i < vertices; i++)
		graph->adjLists[i] = NULL;

	return graph;
}

struct Graph* transformGraph(int** Arr, int size)
{
	struct Graph* graph = createGraph(size);

	for (int i = 0; i < size; i++)
	{
		for (int j = i; j < size; j++)
		{
			if (Arr[i][j] == 1)
			{
				addEdge(graph, i, j);
			}
		}
	}

	return graph;
}

void addEdge(struct Graph* graph, int src, int dest)
{
	struct node* newNode = createNode(dest);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;

	newNode = createNode(src);
	newNode->next = graph->adjLists[dest];
	graph->adjLists[dest] = newNode;
}	

void printGraph(struct Graph* graph)
{
	printf("\nAdjancecy list");

	for (int v = 0; v < graph->numVertices; v++)
	{
		struct node* temp = graph->adjLists[v];
		printf("\n|%d|", v);
		while (temp)
		{
			printf(" -> %d", temp->vertex);
			temp = temp->next;
		}
	}
	printf("\n");
}