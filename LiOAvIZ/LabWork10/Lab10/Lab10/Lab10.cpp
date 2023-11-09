#include "Header.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int sizeMatrix, start = 0, end = 0;
	int** matrix;
	int vertex = 0;
	int weighted = 1;
	int directed = 1;

	pair<int, int> weightedAndDirected = getWeightedAndDirected(argc, argv);
	weighted = weightedAndDirected.first;
	directed = weightedAndDirected.second;
	printf("Enter the size of the adjacency matrix: ");
	scanf("%d", &sizeMatrix);
	printf("Enter from which vertex to count: ");
	scanf("%d", &vertex);
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
	dijkstraAlgorithm(arreyDistance, ancestor, matrix, sizeMatrix, vertex);
	printShortestPath(arreyDistance, sizeMatrix, ancestor, vertex);
	printRadius(matrix, sizeMatrix);
	printDiameter(matrix, sizeMatrix);
	printPeripheralVertex(matrix, sizeMatrix);
	printCentralVertex(matrix, sizeMatrix);
	pair<int, int> c = getCentrHeavin(matrix, sizeMatrix);
	printf("\nCentr heavin vertex: %d      Heavin: %d", c.second, c.first);
	printf("\n");

	for (int i = 0; i < sizeMatrix; i++)
		free(matrix[i]);
	free(matrix);
	free(arreyDistance);
	free(ancestor);

	return 0;
}