#include "Header.h"

int main(void)
{
	srand(time(NULL));
	int sizeMatrix, vertex;
	int** Arr;

	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &sizeMatrix);
	printf("\nEnter vertex: ");
	scanf("%d", &vertex);
	Arr = (int**)malloc(sizeMatrix * sizeof(int*));

	for (int i = 0; i < sizeMatrix; ++i)
	{
		Arr[i] = (int*)malloc(sizeMatrix * sizeof(int));
	}

	printf("Adjacency matrix\n");
	fill_arr(Arr, sizeMatrix);
	print_arr(Arr, sizeMatrix);
	struct Graph* graph = transformGraph(Arr, sizeMatrix);
	printGraph(graph);
	printf("\nDFS in the adjacency matrix\n");
	DFS(Arr, sizeMatrix, vertex);
	printf("\n\nDFS in the adjacency list\n");
	DFSList(graph, sizeMatrix, vertex);
	printf("\n\nIterative DFS in the adjacency matrix\n");
	iterativeDFS(Arr, sizeMatrix, vertex);

	for (int i = 0; i < sizeMatrix; i++)
		free(Arr[i]);
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


void DFS(int** Matrix, int size, int vertex)
{
	int* visited = (int*)calloc(size, size * sizeof(int));
	
	/*for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			printf("\n");
			Dfs(Matrix, size, i, visited);
		}
	}*/
	Dfs(Matrix, size, vertex, visited);

	free(visited);
}

void Dfs(int **Matrix, int size, int vertex, int* visited)
{
	visited[vertex] = 1;
	printf("%d ", vertex);
	for (int i = 0; i < size; i++)
	{
		if (Matrix[vertex][i] == 1 && visited[i] == 0)
		{
			Dfs(Matrix, size, i, visited);
		}
	}
}

void DFSList(struct Graph* graph, int size, int vertex)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	/*for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			printf("\n");
			DfsList(graph, i, visited);
		}
	}*/
	DfsList(graph, vertex, visited);

	free(visited);
}

void DfsList(struct Graph* graph, int vertex, int* visited)
{
	visited[vertex] = 1;
	printf("%d ", vertex);
	struct node* temp = graph->adjLists[vertex];
	while (temp)
	{
		int i = temp->vertex;
		if (visited[i] == 0)
			DfsList(graph, i, visited);
		temp = temp->next;
	}
}

void iterativeDFS(int** Matrix, int size, int vertex)
{
	int* visited = (int*)calloc(size, size * sizeof(int));

	/*for (int i = 0; i < size; i++)
	{
		if (visited[i] == 0) {
			printf("\n");
			iterativeDfs(Matrix, size, i, visited);
		}
	}*/
	iterativeDfs(Matrix, size, vertex, visited);

	free(visited);
}

void iterativeDfs(int** Matrix, int size, int vertex, int* visited)
{
	push(vertex);
	
	while (!stackEmpty())
	{
		vertex = pop();
		if (visited[vertex] == 1)
			continue;
		visited[vertex] = 1;
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