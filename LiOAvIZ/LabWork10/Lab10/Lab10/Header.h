#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 2147483647

void fill_arr(int** Arr, int size, int oriented);
void print_arr(int** Arr, int size);
int* fill_visited(int* visited, int size);
int* fillArreyDistance(int* ArreyDistance, int size);
void shortestPath(int* ArreyDistance, int* predok, int** Matrix, int size, int vertex);
void printArr(int* ArreyDistance, int size);
void printWay(int* Arr, int size, int vertex);
void printShortestPath(int* ArreyDistance, int size, int* predok, int vertex);
int diametr(int** Matrix, int size);
int radius(int** Matrix, int size);
int* getEccentricityArrey(int** Matrix, int size);
void peripheralVertex(int** Matrix, int size);
void centralVertex(int** Matrix, int size);
void printDiametr(int** Matrix, int size);
void printRadius(int** Matrix, int size);