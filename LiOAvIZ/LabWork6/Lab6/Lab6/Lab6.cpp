#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_arr(int** Arr, int size);
void print_arr(int** Arr, int size);
int** vertexIndentification(int** Arr, int size, int u, int v);
struct node* createNode(int v);
struct Graph* createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
struct Graph* transformGraph(int** Arr, int size);

int main(void)
{
	srand(time(NULL));
	int sizeMatrix1, sizeMatrix2;
	int** Arr, **Arrey;

	printf("Enter the size of the adjacency matrix 1: ");
	scanf("%d", &sizeMatrix1);
	Arr = (int**)malloc(sizeMatrix1 * sizeof(int*));

	for (int i = 0; i < sizeMatrix1; ++i)
	{
		Arr[i] = (int*)malloc(sizeMatrix1 * sizeof(int));
	}

	printf("Enter the size of the adjacency matrix 2: ");
	scanf("%d", &sizeMatrix2);
	Arrey = (int**)malloc(sizeMatrix2 * sizeof(int*));

	for (int i = 0; i < sizeMatrix2; ++i)
	{
		Arrey[i] = (int*)malloc(sizeMatrix2 * sizeof(int));
	}

	printf("Adjacency matrix 1\n");
	fill_arr(Arr, sizeMatrix1);
	print_arr(Arr, sizeMatrix1);
	printf("\n\nAdjacency matrix 2\n");
	fill_arr(Arrey, sizeMatrix2);
	print_arr(Arrey, sizeMatrix2);

	struct Graph* graph1 = transformGraph(Arr, sizeMatrix1);
	struct Graph* graph2 = transformGraph(Arrey, sizeMatrix2);

	printGraph(graph1);
	printGraph(graph2);

	Arr = vertexIndentification(Arr, sizeMatrix1, 1, 2);
	print_arr(Arr, sizeMatrix1-1);

	free(Arr);
	free(Arrey);

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

int** vertexIndentification(int** Arr, int size, int u, int v)
{
	int** ArreyIndentification;
	int* ArrU = (int*)malloc(size * sizeof(int));
	int* ArrV = (int*)malloc(size * sizeof(int));
	int* ArrZ = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		ArrU[i] = Arr[u][i];
		ArrV[i] = Arr[v][i];
	}

	for (int i = 0; i < size; i++)
	{
		ArrZ[i] = ArrU[i] || ArrV[i];
		
	}

	ArrZ[size - 2] = ArrZ[size-1];

	for (int i = 0; i < size; i++)
	{
		Arr[u][i] = 0;
		Arr[i][u] = 0;

		Arr[v][i] = 0;
		Arr[i][v] = 0;
	}

	for (int i = 0; i < size; i++)
	{
		Arr[v][i] = Arr[size-1][i];
		Arr[i][v] = Arr[i][size-1];
	}

	ArreyIndentification = (int**)malloc((size - 1) * sizeof(int*));

	for (int i = 0; i < (size - 1); ++i)
	{
		ArreyIndentification[i] = (int*)malloc((size - 1) * sizeof(int));
	}

	for (int i = 0; i < (size - 1); i++)
	{
		for (int j = 0; j < (size - 1); j++)
		{
			ArreyIndentification[i][j] = Arr[i][j];
		}
	}

	for (int i = 0; i < (size - 1); i++)
	{
		if (u == i)
		{
			ArreyIndentification[u][i] = 0;
			continue;
		}
		ArreyIndentification[u][i] = ArrZ[i];
		ArreyIndentification[i][u] = ArrZ[i];
	}

	return ArreyIndentification;
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
				Arr[i][j] = 0;
				Arr[j][i] = 0;
			}
			else 
			{
				Arr[i][j] = 1;
				Arr[j][i] = 1;
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