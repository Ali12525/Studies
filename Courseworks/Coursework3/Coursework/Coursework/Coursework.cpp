#include "Header.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int sizeFirstShare = 4;
	int sizeSecondShare = 8;
	int manually = 0;
	printf("Enter size first share: ");
	scanf("%d", &sizeFirstShare);
	printf("Enter size second share: ");
	scanf("%d", &sizeSecondShare);
	int* pair = (int*)malloc(sizeSecondShare * sizeof(int));
	struct Graph* graph = createGraph(sizeFirstShare);
	printf("Fill graph manually?(0 - no): ");
	scanf("%d", &manually);
	if (manually == 0)
		fillGraphAuto(graph, sizeFirstShare, sizeSecondShare);
	else
		fillGraphManually(graph, sizeFirstShare, sizeSecondShare);
	printGraph(graph);
	//clock_t start = clock();
	kuhnAlgorithm(graph, pair, sizeFirstShare, sizeSecondShare);
	//clock_t end = clock();
	printKun(pair, sizeSecondShare);
	/*double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\ntime: %f s.\n\n", time_spent);*/

	free(pair);
	freeGraph(graph, sizeFirstShare);

	return 0;
}