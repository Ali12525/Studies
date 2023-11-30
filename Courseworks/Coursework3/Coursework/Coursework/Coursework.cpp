#include "Header.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int sizeFirstShare = 5;
	int sizeSecondShare = 8;
	int manually = 0;
	int* pair = (int*)malloc(sizeSecondShare * sizeof(int));
	struct Graph* graph = createGraph(sizeFirstShare);
	printf("Fill graph manually: ");
	scanf("%d", &manually);
	if (manually == 0)
		fillGraphAuto(graph, sizeFirstShare, sizeSecondShare);
	else
		fillGraphManually(graph, sizeFirstShare, sizeSecondShare);
	printGraph(graph);
	clock_t start = clock();
	kuhnAlgorithm(graph, pair, sizeFirstShare, sizeSecondShare);
	clock_t end = clock();
	printKun(pair, sizeSecondShare);
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\ntime: %f s.\n\n", time_spent);

	free(pair);
	freeGraph(graph, sizeFirstShare);

	return 0;
}