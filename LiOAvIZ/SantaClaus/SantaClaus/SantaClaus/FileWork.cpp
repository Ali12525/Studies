#include "FileWork.h"

int** readMatrixFromFile(std::string name, int total, int sizeInStr)
{
	std::string line;
	std::ifstream in(name);
	int** array = new int* [total];
	int fl = 0;

	if (in.is_open())
	{
		int i = 0;

		while (std::getline(in, line))
		{
			array[i] = new int[sizeInStr];
			stringstream text_stream(line);
			string item;
			int j = 0;

			while (std::getline(text_stream, item, ','))
			{
				if (fl == 0)
				{
					fl = 1;
					continue;
				}
				array[i][j] = stoi(item);
				j++;
			}

			fl = 0;
			i++;
		}
	}

	in.close();

	return array;
}

void writeMatrixInFile(int** array, int total)
{
	std::ofstream out;
	out.open("Out.csv");
	if (out.is_open())
	{
		out << "ChildId   GiftId" << std::endl;

		for (size_t i = 0; i < total; i++)
		{
			out << array[i][0] << ",    " << array[i][1];
			out << std::endl;
		}

	}
	out.close();
	std::cout << "File has been written" << std::endl;
}