#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void excercise1();
void excercise2();
void excercise3();
void excercise4();
void excercise5();

int main()
{
	srand(time(NULL));
	excercise1();
	excercise2();
	excercise3();
	excercise4();
	excercise5();

	return 0;
}

void excercise1()
{
	int Arr[10] = {0};
	int max = -1000;
	int min = 1000;

	printf("excercise 1:\n");
	printf("Arr: ");

	for (int i = 0; i < 10; i++)
	{
		Arr[i] = rand() % 100;
		printf("%d ", Arr[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		if (min > Arr[i])
		{
			min = Arr[i];
		}
		if (max < Arr[i])
		{
			max = Arr[i];
		}
	}

	printf("\nmax = %d\nmin = %d", max, min);

	printf("\ndifference: %d", max - min);
}

void excercise2()
{
	int Arr[10] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		Arr[i] = rand() % 100;
	}

	printf("\n\n\nexcercise 2:\nArr: ");
	for (int i = 0; i < 10; i++)
	{
		printf("%d ", Arr[i]);
	}

}

void excercise3()
{
	int* Arr;
	int n;

	printf("\n\n\nexcercise 3:\nEnter the size of array: ");
	scanf("%d", &n);
	Arr = (int*)malloc(n * 4);

	printf("Arr: ");
	for (int i = 0; i < n; i++)
	{
		Arr[i] = rand() % 100;
		printf("%d ", Arr[i]);
	}
}

void excercise4()
{
	int result = 0;
	int Arr[3][3];
	printf("\n\n\nexcercise 4:\n");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Arr[i][j] = rand() % 100;
			result += Arr[i][j];
			printf("%3d ", Arr[i][j]);
		}
		printf("result = %d", result);
		result = 0;
		printf("\n");
	}
}

void excercise5()
{
	typedef struct student
	{
		char name[20];
		char secondName[20];
		char facult[20];
		char nomzach[20];
	}Student;

	char name[20];
	char secondName[20];

	Student student[3];
	student[0] = { "Vanya", "Andreevich", "FVT", "555"};
	student[1] = { "Petya", "Olegovich", "FITE", "333"};
	student[2] = { "Andrey", "Raskolnikov", "FPTET"};

	printf("\n\nexcercise 5:\n");
	printf("\nEnter the student's first and last name: ");
	scanf("%s %s", name, secondName);

	for (int i = 0; i < 3; i++)
	{
		if (strcmp(student[i].name,name) == 0 and strcmp(student[i].secondName,secondName) == 0)
		{
			printf("\nStudent %s %s number %d, studies at the faculty %s with record book number %s\n", student[i].name, student[i].secondName, i, student[i].facult, student[i].nomzach);
		}
	}





}