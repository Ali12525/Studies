#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_arr(int** Arr, int size);
void print_arr(int** Arr, int size);
int** vertexIdentification2(int** Arr, int size, int u, int v);
int** vertexSplitting(int** Arr, int size, int u, int v);
int find_arr(int* Arr, int size, int element);
int** vertexIdentification1(int** matrix, int size, int u, int v);
struct node* createNode(int v);
struct Graph* createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
struct Graph* transformGraph(int** Arr, int size);
struct Graph* delVertexGraph(int** Arr, int size, int u);
struct Graph* vertexIndentificationMatrix(struct Graph* graph, int** Arrey, int size, int u, int v);
void printGraphDel(struct Graph* graph, int u, int k);

int main(void)
{
	srand(time(NULL));
	int sizeMatrix1, sizeMatrix2;
	int** Arr, **Arrey;
	char ch;
	int ext = 0, extt = 0, priority = -1;
	int u, v;

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
	printf("Press any key to exit");
	getchar();
	getchar();

	do
	{
		system("cls");
		printf("1. Vertex indentification (Matrix)\n2. Vertex contraction (Matrix)\n3. Vertex splitting (Matrix)\n4. Vertex indentification (list)\n5. Vertex contraction (list)\n6. Vertex splitting (list)\n");
		printf("Select item: ");
		scanf("%c", &ch);
		system("cls");
		if (ch != '\n')
		{
			scanf("%*[^\n]");
			getchar();
		}

		switch (ch)
		{
		case '1':
			printf("Adjacency matrix 1\n");
			print_arr(Arr, sizeMatrix1);
			printf("\n\nAdjacency matrix 2\n");
			print_arr(Arrey, sizeMatrix2);
			printf("Enter vertex: \n");
			printf("u = ");
			scanf("%d", &u);
			printf("v = ");
			scanf("%d", &v);
			Arr = vertexIdentification1(Arr, sizeMatrix1, u, v);
			Arrey = vertexIdentification1(Arrey, sizeMatrix2, u, v);
			printf("Result:\n");
			print_arr(Arr, sizeMatrix1 - 1);
			printf("\n");
			print_arr(Arrey, sizeMatrix1 - 1);
			printf("Press any key to exit");
			getchar();
			getchar();
			break;
		case '2':
			printf("Adjacency matrix 1\n");
			print_arr(Arr, sizeMatrix1);
			printf("\n\nAdjacency matrix 2\n");
			print_arr(Arrey, sizeMatrix2);
			printf("Enter vertex: \n");
			printf("u = ");
			scanf("%d", &u);
			printf("v = ");
			scanf("%d", &v);
			if (Arr[u][v] == 0)
			{
				printf("Error\n");
				printf("Press any key to exit");
				getchar();
				getchar();
				break;
			}
			Arr = vertexIdentification1(Arr, sizeMatrix1, u, v);
			Arrey = vertexIdentification1(Arrey, sizeMatrix2, u, v);
			printf("Result:\n");
			print_arr(Arr, sizeMatrix1);
			printf("\n");
			print_arr(Arrey, sizeMatrix1);
			printf("Press any key to exit");
			getchar();
			getchar();
			break;
		case '3':
			printf("Adjacency matrix 1\n");
			print_arr(Arr, sizeMatrix1);
			printf("\n\nAdjacency matrix 2\n");
			print_arr(Arrey, sizeMatrix2);
			printf("Enter vertex: \n");
			printf("u = ");
			scanf("%d", &u);
			printf("v = ");
			scanf("%d", &v);
			Arr = vertexSplitting(Arr, sizeMatrix1, u, v);
			Arrey = vertexSplitting(Arrey, sizeMatrix2, u, v);
			printf("Result:\n");
			print_arr(Arr, sizeMatrix1 + 1);
			printf("\n");
			print_arr(Arrey, sizeMatrix1 + 1);
			printf("Press any key to exit");
			getchar();
			getchar();
			break;
		case '4':
			printf("Enter vertex: \n");
			printf("u = ");
			scanf("%d", &u);
			printf("v = ");
			scanf("%d", &v);
			graph1 = vertexIndentificationMatrix(graph1, Arr, sizeMatrix1, u, v);
			printGraphDel(graph1, u , v);
			printf("\n");
			getchar();
			getchar();
			break;
		case '5':

			break;
		case '6':

			break;
		default:
			break;
		}
	} while (ext == 0);

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

int** vertexIdentification2(int** Arr, int size, int u, int v)
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

	for (int i = 0; i < size; i++)
	{
		Arr[u][i] = 0;
		Arr[i][u] = 0;

		Arr[v][i] = 0;
		Arr[i][v] = 0;
	}

	ArreyIndentification = (int**)malloc(size * sizeof(int*));

	for (int i = 0; i < size; ++i)
	{
		ArreyIndentification[i] = (int*)malloc(size * sizeof(int));
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			ArreyIndentification[i][j] = Arr[i][j];
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (u == i)
		{
			ArreyIndentification[u][i] = 0;
			continue;
		}
		if (v == i)
		{
			ArreyIndentification[v][i] = 0;
			ArreyIndentification[i][v] = 0;
			continue;
		}
		ArreyIndentification[u][i] = ArrZ[i];
		ArreyIndentification[i][u] = ArrZ[i];
	}

	return ArreyIndentification;
}

int** vertexIdentification1(int** Arr, int size, int u, int v)
{
	int** ArreyIndentification;
	ArreyIndentification = (int**)malloc((size - 1) * sizeof(int*));

	for (int i = 0; i < size - 1; ++i)
	{
		ArreyIndentification[i] = (int*)malloc((size - 1) * sizeof(int));
	}

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			ArreyIndentification[i][j] = 0;
		}
	}

	if (v < u)
	{
		int t = u;
		u = v;
		v = t;
	}

	int di = 0, dj = 0;
	for (int i = 0; i < size; i++)
	{
		if (i == u || i == v)
			i++;
		if (i == u || i == v)
			i++;

		if (i >= size)
			break;

		for (int j = 0; j < size; j++)
		{
			if (j == u || j == v)
				j++;
			if (j == u || j == v)
				j++;

			if (j >= size)
				break;

			ArreyIndentification[di][dj] = Arr[i][j];
			dj++;
		}

		di++;
		dj = 0;
	}

	int k = 0;

	for (int i = 0; i < size; i++)
	{
		if (i == u || i == v)
			i++;
		if (i == u || i == v)
			i++;
		if (i >= size)
			break;
		if (Arr[v][i] == 1)
		{
			ArreyIndentification[size - 2][k] = 1;
			ArreyIndentification[k][size - 2] = 1;
		}
		if (Arr[u][i] == 1)
		{
			ArreyIndentification[size - 2][k] = 1;
			ArreyIndentification[k][size - 2] = 1;
		}
		k++;
	}

	return ArreyIndentification;
}


int** vertexSplitting(int** Arr, int size, int u, int v)
{
	int** ArreyIndentification;

	ArreyIndentification = (int**)malloc((size + 1) * sizeof(int*));

	for (int i = 0; i < size + 1; ++i)
	{
		ArreyIndentification[i] = (int*)malloc((size + 1) * sizeof(int));
	}

	for (int i = 0; i < size + 1; i++)
	{
		for (int j = 0; j < size + 1; j++)
		{
			if (i == size || j == size) {
				ArreyIndentification[i][j] = 0;
				continue;
			}
			ArreyIndentification[i][j] = Arr[i][j];
		}
	}

	ArreyIndentification[u][v] = 0;
	ArreyIndentification[v][u] = 0;

	if (Arr[u][v] == 1)
	{
		ArreyIndentification[u][size] = 1;
		ArreyIndentification[size][u] = 1;
		ArreyIndentification[v][size] = 1;
		ArreyIndentification[size][v] = 1;
	}

	return ArreyIndentification;
}

struct Graph* vertexIndentificationMatrix(struct Graph* graph, int** Arrey, int size, int u, int v)
{
	int count = 0, count2 = 0;
	struct node* temp = graph->adjLists[v];
	int* Arr = (int*)malloc(count * sizeof(int));
	int* Arr2 = (int*)malloc(count2 * sizeof(int));

	while (temp)
	{
		count++;
		Arr = (int*)realloc(Arr, count * sizeof(int));
		Arr[count - 1] = temp->vertex;

		temp = temp->next;
	}

	temp = graph->adjLists[u];

	while (temp)
	{
		if (temp->vertex == v)
		{
			temp = temp->next;
			continue;
		}
		if (find_arr(Arr, count, temp -> vertex) == 1)
		{
			count2++;
			Arr2 = (int*)realloc(Arr2, count2 * sizeof(int));
			Arr2[count2 - 1] = temp->vertex;
		}
		temp = temp->next;
	}

	graph = delVertexGraph(Arrey, size, u);
	printGraph(graph);

	for (int i = 0; i < count2; i++)
	{
		addEdge(graph, v, Arr2[i]);
	}

	struct Graph* graph2 = createGraph(size - 1);
	int k = 0;

	for (int i = 0; i < graph2->numVertices + 1; i++)
	{
		if (i == u)
		{
			continue;
		}
		struct node* temp = graph->adjLists[i];
		graph2->adjLists[k] = temp;
		k++;
	}

	printGraph(graph);

	return graph2;
}

struct Graph* delVertexGraph(int** Arr, int size, int u)
{
	struct Graph* graph = createGraph(size);

	for (int i = 0; i < size; i++)
	{
		if (i == u)
			continue;
		for (int j = i; j < size; j++)
		{
			if (j == u)
				continue;
			if (Arr[i][j] == 1)
			{
				addEdge(graph, i, j);
			}
		}
	}

	return graph;
}

int find_arr(int* Arr, int size, int element)
{
	for (int j = 0; j < size; j++)
	{
		if (element == Arr[j])
		{
			return 0;
		}
	}	

	return 1;
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

void printGraphDel(struct Graph* graph, int u, int k)
{
	printf("\nAdjancecy list");

	for (int v = 0; v < graph->numVertices; v++)
	{
		struct node* temp = graph->adjLists[v];
		
		if (v == k - 1)
		{
			printf("\n|%d%d|", u, k);
		}
		else if (v >= u)
		{
			printf("\n|%d|", v + 1);
		}
		else
		{
			printf("\n|%d|", v);
		}
	
		while (temp)
		{
			printf(" -> %d", temp->vertex);
			temp = temp->next;
		}
	}
	printf("\n");
}