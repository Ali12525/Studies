#include "Header.h"

void freeGraph(struct Graph* graph, int size)
{
	for (int i = 0; i < size; i++)
	{
		struct node* temp = graph->adjLists[i];

		while (temp)
		{
			struct node* p = temp;
			temp = temp->next;
			free(p);
		}

		free(temp);
	}
	free(graph->adjLists);
	free(graph);
}

void fillGraphAuto(struct Graph* graph, int sizeFirstShare, int sizeSecondShare)
{
	for (int i = 0; i < sizeFirstShare; i++)
	{
		for (int j = 0; j < rand() % sizeSecondShare; j++)
		{
			int v;
			do
			{
				v = rand() % sizeSecondShare;
			} while (total(graph, i, v) == 1);

			addEdge(graph, i, v);
		}
	}
}

void fillGraphManually(struct Graph* graph, int sizeFirstShare, int sizeSecondShare)
{
	int v;
	printf("Enter -1 for next vertex in first share\n");

	for (int i = 0; i < sizeFirstShare; i++)
	{
		while (1)
		{
			do
			{
				printf("%d -> ", i);
				scanf("%d", &v);
				if (total(graph, i, v) == 1)
					printf("such a connection already exists\n");
			} while (total(graph, i, v) == 1);

			if (v == -1)
				break;
			if (v >= sizeSecondShare)
			{
				printf("such vertex in second share no\n");
				continue;
			}
			addEdge(graph, i, v);
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

void addEdge(struct Graph* graph, int src, int dest)
{
	struct node* newNode = createNode(dest);
	newNode->next = graph->adjLists[src];
	graph->adjLists[src] = newNode;
}

void printGraph(struct Graph* graph)
{
	printf("\nGraph:");

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

int total(struct Graph* graph, int src, int vertex)
{
	int count = 0;

	struct node* temp = graph->adjLists[src];

	while (temp)
	{
		if (temp->vertex == vertex)
			return 1;
		temp = temp->next;
	}

	return 0;
}