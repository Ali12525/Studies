#include "Header.h"

int main()
{
	srand(time(NULL));

	clock_t start, end;

	

	int** children = readMatrixFromFile("kids_wish.csv", CHILD_COUNT, CHILD_PRESENTS);
	//Print2dArray(children, CHILD_COUNT, CHILD_PRESENTS);

	int** presents = readMatrixFromFile("ded_moroz_wish.csv", PRESENTS_COUNT, PRESENTS_CHILD);
	//Print2dArray(presents, PRESENTS_COUNT, PRESENTS_CHILD);

	start = clock();

	int*** weightChildren = Make3dArrayFrom2d(children, CHILD_COUNT, CHILD_PRESENTS);
	//Print3dArray(weightChildren, CHILD_COUNT, CHILD_PRESENTS);

	std::cout << "\n" << std::endl;

	int*** weightPresents = Make3dArrayFrom2d(presents, PRESENTS_COUNT, PRESENTS_CHILD);
	//Print3dArray(weightPresents, PRESENTS_COUNT, PRESENTS_CHILD);

	std::cout << "start calc" << std::endl;

	int unis = 0;

	int** table = UNISTable(weightChildren, weightPresents, &unis);

	//Print2dArray(table, CHILD_COUNT, 3);

	end = clock();

	std::cout << unis << " - unis " << (((double)end - start) / (double)CLOCKS_PER_SEC) / 60 << " min" << std::endl;

	writeMatrixInFile(table, CHILD_COUNT);

	return 0;
}