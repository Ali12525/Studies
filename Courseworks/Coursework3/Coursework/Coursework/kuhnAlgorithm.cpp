#include "Header.h"

void kuhnAlgorithm(struct Graph* graph, int* pair, int sizeFirstShare, int sizeSecondShare)
{
	int* used1 = (int*)calloc(sizeFirstShare, sizeFirstShare * sizeof(int));
	pair = fillArrey(pair, sizeSecondShare, -1);

	for (int i = 0; i < sizeFirstShare; i++)
	{
		struct node* temp = graph->adjLists[i];
		while (temp)
		{
			int v = temp->vertex;
			if (pair[v] == -1)
			{
				pair[v] = i;
				used1[i] = 1;
				break;
			}
			temp = temp->next;
		}
	}

	int* visited = (int*)calloc(sizeFirstShare, sizeFirstShare * sizeof(int));

	for (int i = 0; i < sizeFirstShare; i++)
	{
		if (used1[i])
			continue;
		if (dfs(graph, i, visited, pair))
			visited = fillArrey(visited, sizeFirstShare, 0);
	}

	free(visited);
	free(used1);
}

int dfs(struct Graph* graph, int vertex, int* visited, int* pair)
{
	if (visited[vertex])
		return 0;
	visited[vertex] = 1;

	struct node* temp = graph->adjLists[vertex];

	while (temp)
	{
		int v = temp->vertex;
		if (pair[v] == -1 || dfs(graph, pair[v], visited, pair))
		{
			pair[v] = vertex;
			return 1;
		}
		temp = temp->next;
	}

	free(temp);

	return 0;
}

void printKun(int* arr, int sizeSecondShare)
{
	printf("\nGreatest matching:\n");
	for (int i = 0; i < sizeSecondShare; i++)
	{
		if (arr[i] == -1)
			continue;
		printf("%d - %d\n", arr[i], i);
	}
}

/*pair = fillArrey(pair, size, -1);
	for (int i = 0; i < size; i++)
	{
		if(dfs(graph, i, visited, pair))
			visited = fillArrey(visited, size, 0);
}*/