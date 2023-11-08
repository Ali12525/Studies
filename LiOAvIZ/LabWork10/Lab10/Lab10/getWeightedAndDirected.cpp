#include "Header.h"

pair<int, int> getWeightedAndDirected(int argc, char* argv[])
{
	int weighted = 1;
	int directed = 1;
	if (argc > 1)
	{
		for (int i = 0; i < argc; ++i)
		{
			if (strcmp(argv[i], "-weighted") == 0)
				weighted = std::stoi(argv[i + 1]);

			if (strcmp(argv[i], "-directed") == 0)
				directed = std::stoi(argv[i + 1]);
		}
	}
	else
	{
		printf("Enter weighted: ");
		scanf("%d", &weighted);
		getchar();
		printf("Enter directed: ");
		scanf("%d", &directed);
		getchar();
	}

	return pair<int, int>(weighted, directed);
}