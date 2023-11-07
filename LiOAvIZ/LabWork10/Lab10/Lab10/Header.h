#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <string>

#define MAX 2147483647

void fillMatrix(int** matrix, int size, int directed, int weighted);
void printMatrix(int** matrix, int size);
int* fillArreyDistance(int* arreyDistance, int size);
void shortestPath(int* arreyDistance, int* predok, int** matrix, int size, int vertex);
void printArr(int* arreyDistance, int size);
void printWay(int* arr, int size, int vertex);
void printShortestPath(int* arreyDistance, int size, int* ancestor, int vertex);
int diameter(int** matrix, int size);
int radius(int** matrix, int size);
int* getEccentricityArrey(int** matrix, int size);
void peripheralVertex(int** matrix, int size);
void centralVertex(int** matrix, int size);
void printDiameter(int** matrix, int size);
void printRadius(int** matrix, int size);