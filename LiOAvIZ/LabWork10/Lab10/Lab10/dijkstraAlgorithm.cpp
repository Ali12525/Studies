#include "Header.h"

void dijkstraAlgorithm(int* arreyDistance, int* ancestor, int** matrix, int size, int vertex)
{
	std::priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
	arreyDistance[vertex] = 0;
	ancestor[vertex] = vertex;
	queue.push({ 0, vertex });

	while (!queue.empty())
	{
		pair<int, int> c = queue.top();
		queue.pop();
		vertex = c.second;
		int weight = c.first;
		if (weight > arreyDistance[vertex])
			continue;
		for (int i = 0; i < size; i++)
		{
			if (matrix[vertex][i] != MAX)
			{
				int v = i;
				int len = matrix[vertex][i];
				if (arreyDistance[v] > arreyDistance[vertex] + len) {
					ancestor[v] = vertex;
					arreyDistance[v] = arreyDistance[vertex] + len;
					queue.push({ arreyDistance[v], v });
				}
			}
		}
	}
}