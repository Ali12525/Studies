#include "Constants.h"
#include "QsortRecursive.h"
#include "UNISTable.h"

int** UNISTable(int*** children, int*** presents, int* unis)
{
	int** table = new int* [CHILD_COUNT];
	for (size_t i = 0; i < CHILD_COUNT; i++)
		table[i] = new int[3] { 0, 0, 0 };

	// p1
	std::map<int, int> allPresents;
	for (size_t i = 0; i < PRESENTS_COUNT; i++)
	{
		allPresents[i] = 250;
	}
	std::cout << "1 part" << std::endl;

	//p2 child - present
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		int** curChild = children[i];
		for (size_t j = 0; j < CHILD_PRESENTS * 0.3; j++)
		{
			int curPresent = curChild[j][0];
			int weightCur = FindChildrenInPresent(presents, curPresent, i, allPresents) + curChild[j][1];

			if (i >= CHILD_TWEENS)
			{
				if (table[i][2] < weightCur)
				{
					table[i][0] = i;
					table[i][1] = curPresent;
					table[i][2] = weightCur;
				}
			}
			else
			{
				int indSec;
				if (i % 2 == 0)
					indSec = i + 1;
				else
					indSec = i - 1;

				int** secChild = children[indSec]; 
				int l = -1;
				for (int m = 0; m < CHILD_PRESENTS; m++)
				{
					if (secChild[m][0] == curPresent)
						l = m;
					break;
				}

				int weightSec;
				if (l >= 0)
					weightSec = FindChildrenInPresent(presents, curPresent, i + 1, allPresents) + secChild[l][1];
				else
					weightSec = FindChildrenInPresent(presents, curPresent, i + 1, allPresents) - 1;

				if ((table[i][2] + table[indSec][2]) < weightSec + weightCur)
				{
					table[i][0] = i;
					table[i][1] = curPresent;
					table[i][2] = weightCur;

					table[indSec][0] = indSec;
					table[indSec][1] = curPresent;
					table[indSec][2] = weightSec;
				}
			}

		}
	}
	std::cout << "2 part" << std::endl;

	//p3 present - child
	for (size_t i = 0; i < PRESENTS_COUNT; i++)
	{
		int** curPresent = presents[i];
		for (size_t j = 0; j < PRESENTS_CHILD * 0.2; j++)
		{
			int curChild = curPresent[j][0];
			int weightCur = FindPresentInChildren(children, curChild, i, allPresents) + curPresent[j][1];

			if (curChild >= CHILD_TWEENS)
			{
				if (table[curChild][2] < weightCur)
				{
					table[curChild][0] = curChild;
					table[curChild][1] = i;
					table[curChild][2] = weightCur;
				}
			}
			else
			{
				int secChild;
				if (curChild % 2 == 0)
					secChild = curChild + 1;
				else
					secChild = curChild - 1;

				//    
				int l = -1;
				for (int m = 0; m < PRESENTS_CHILD; m++)
				{
					if (curPresent[m][0] == secChild)
						l = m;
					break;
				}

				int weightSec;
				if (l >= 0)
					weightSec = FindPresentInChildren(children, secChild, i, allPresents) + curPresent[l][1];
				else
					weightSec = FindPresentInChildren(children, secChild, i, allPresents) - 1;

				if (table[curChild][2] + table[secChild][2] < weightCur + weightSec)
				{
					table[curChild][0] = curChild;
					table[curChild][1] = i;
					table[curChild][2] = weightCur;

					table[secChild][0] = secChild;
					table[secChild][1] = i;
					table[secChild][2] = weightSec;
				}
			}
		}
	}
	std::cout << "3 part" << std::endl;

	//p4 tweens 1
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][0] >= CHILD_TWEENS)
			continue;

		if (allPresents[table[i][1]] <= 0)
		{
			table[i][2] = 0;

			int ind2;
			if (table[i][0] % 2 == 0)
				ind2 = table[i][0] + 1;
			else
				ind2 = table[i][0] - 1;

			for (size_t j = 0; j < CHILD_COUNT; j++)
			{
				if (table[j][0] == ind2)
				{
					table[j][2] = 0;
					break;
				}
			}
		}
		else
			allPresents[table[i][1]] -= 1;
	}
	std::cout << "4 part" << std::endl;

	// p5 tweens lasts
	for (size_t i = 0; i < CHILD_TWEENS; i++)
	{
		// childrens
		if (table[i][2] == 0)
		{
			int** curChild = children[table[i][0]];

			for (size_t j = 0; j < CHILD_PRESENTS; j++)
			{
				int curPresent = curChild[j][0];
				if (allPresents[curPresent] % 2 != 0 || allPresents[curPresent] <= 0)
					continue;

				int weightCur = FindChildrenInPresent(presents, curPresent, i, allPresents) + curChild[j][1];
				int indSec;
				if (i % 2 == 0)
					indSec = i + 1;
				else
					indSec = i - 1;

				int** secChild = children[indSec];
  
				int l = -1;
				for (int m = 0; m < CHILD_PRESENTS; m++)
				{
					if (secChild[m][0] == curPresent)
						l = m;
					break;
				}

				int weightSec;
				if (l >= 0)
					weightSec = FindChildrenInPresent(presents, curPresent, i + 1, allPresents) + secChild[l][1];
				else
					weightSec = FindChildrenInPresent(presents, curPresent, i + 1, allPresents) - 1;

				if ((table[i][2] + table[indSec][2]) < weightSec + weightCur)
				{
					table[i][0] = i;
					table[i][1] = curPresent;
					table[i][2] = weightCur;

					table[indSec][0] = indSec;
					table[indSec][1] = curPresent;
					table[indSec][2] = weightSec;

					allPresents[curPresent] -= 2;
				}
			}
		}

		// presents
		if (table[i][2] == 0)
		{
			for (size_t j = 0; j < PRESENTS_COUNT; j++)
			{

				if (allPresents[j] <= 0 && allPresents[j] % 2 == 0)
				{
					table[i][1] = j;
					table[i][2] = -10;
					if (i % 2 == 0)
					{
						table[i + 1][1] = j;
						table[i + 1][2] = -10;
					}
					else
					{
						table[i - 1][1] = j;
						table[i - 1][2] = -10;
					}

					allPresents[j] -= 2;
				}
			}
		}
	}

	// p6
	qsortRecursive(table, CHILD_COUNT, 2, false);
	std::cout << "6 part" << std::endl;

	// p7
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][0] < CHILD_TWEENS)
			continue;

		if (allPresents[table[i][1]] <= 0)
			table[i][2] = 0;
		else
			allPresents[table[i][1]] -= 1;
	}
	std::cout << "7 part" << std::endl;

	// p8
	qsortRecursive(table, CHILD_COUNT, 0, true);
	std::cout << "8 part" << std::endl;

	// p9 child - present
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][2] == 0)
		{
			bool flag = false;
			for (size_t j = 0; j < CHILD_PRESENTS; j++)
			{
				int child = table[i][0];
				int* presentInChildList = children[child][j];

				if (allPresents[presentInChildList[0]] <= 0)
					continue;

				int weightPresent = FindChildrenInPresent(presents, presentInChildList[0], child, allPresents);
				int weightChildren = children[child][j][1];

				int weight = weightPresent + weightChildren;

				if (table[i][2] < weight)
				{
					table[i][0] = child;
					table[i][1] = presentInChildList[0];
					table[i][2] = weight;
					flag = true;
				}
			}

			if (flag)
				allPresents[table[i][1]] -= 1;

		}
	}
	std::cout << "9 part" << std::endl;

	// p10 present - child
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][2] == 0)
		{
			bool flag = false;
			for (size_t j = 0; j < PRESENTS_CHILD; j++)
			{
				int present = table[i][1];
				int* childInPresentList = presents[present][j];

				if (allPresents[present] <= 0)
					continue;

				int weightChildren = FindPresentInChildren(children, childInPresentList[0], present, allPresents);
				int weightPresent = presents[present][j][1];

				int weight = weightPresent + weightChildren;

				if (table[i][2] < weight)
				{
					table[i][0] = childInPresentList[0];
					table[i][1] = present;
					table[i][2] = weight;
					flag = true;
				}
			}

			if (flag)
				allPresents[table[i][1]] -= 1;
		}
	}
	std::cout << "10 part" << std::endl;

	// p11 refill presents
	for (size_t i = 0; i < PRESENTS_COUNT; i++)
	{
		allPresents[i] = PRESENTS_CHILD;
	}

	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][2] == 0)
			continue;
		allPresents[table[i][1]] -= 1;
	}
	std::cout << "11 part" << std::endl;

	// p12 fill lasts
	for (size_t i = 0; i < CHILD_COUNT; i++)
	{
		if (table[i][2] == 0)
		{
			for (size_t j = 0; j < PRESENTS_COUNT; j++)
			{
				if (allPresents[j] > 0)
				{
					table[i][1] = j;
					table[i][2] = FindPresentInChildren(children, table[i][0], j, allPresents) +
						FindChildrenInPresent(presents, j, table[i][0], allPresents);
					allPresents[j] -= 1;
					break;
				}
			}
		}
	}
	std::cout << "12 part" << std::endl;

	qsortRecursive(table, CHILD_COUNT, 0, true);

	// calc unis
	for (int i = 0; i < CHILD_COUNT; i++)
	{
		*unis += table[i][2];
	}
	*unis /= CHILD_COUNT;

	return table;
}

int FindChildrenInPresent(int*** presents, int presentNum, int children, std::map<int, int> allPresents)
{
	int res = -1;

	if (allPresents[presentNum] <= 0)
		return -10000;

	for (size_t i = 0; i < PRESENTS_CHILD; i++)
	{
		if (presents[presentNum][i][1] <= 0)
			return -1;

		if (presents[presentNum][i][0] == children)
		{
			return presents[presentNum][i][1];
		}
	}

	return -1;
}

int FindPresentInChildren(int*** children, int childNum, int present, std::map<int, int> allPresents)
{
	int res = -1;

	if (allPresents[present] <= 0)
		return -10000;

	for (size_t i = 0; i < CHILD_PRESENTS; i++)
	{
		if (children[childNum][i][1] <= 0)
			return -1;

		if (children[childNum][i][0] == present)
		{
			return children[childNum][i][1];
		}
	}

	return -1;
}