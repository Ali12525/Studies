#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 2147483647

void fill_arr(int** Arr, int size);
void print_arr(int** Arr, int size);
int Bfs(int** Matrix, int size, int vertex, int end, int shutdown);
int* fill_visited(int* visited, int size);
int* fillArreyDistance(int* ArreyDistance, int size);
void shortestPath(int** Matrix, int size, int vertex);
void printArr(int* ArreyDistance, int size);