#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <string>
using namespace std;

#define MAX 2147483647

void fillMatrix(int** matrix, int size, int directed, int weighted);
int* fillArreyDistance(int* arreyDistance, int size);
void dijkstraAlgorithm(int* arreyDistance, int* predok, int** matrix, int size, int vertex);
int getDiameter(int** matrix, int size);
int getRadius(int** matrix, int size);
int* getEccentricityArrey(int** matrix, int size);
void printMatrix(int** matrix, int size);
void printArr(int* arreyDistance, int size);
void printWay(int* arr, int size, int vertex);
void printShortestPath(int* arreyDistance, int size, int* ancestor, int vertex);
void printDiameter(int** matrix, int size);
void printRadius(int** matrix, int size);
void printPeripheralVertex(int** matrix, int size);
void printCentralVertex(int** matrix, int size);
pair<int, int> getWeightedAndDirected(int argc, char* argv[]);
pair<int, int> getCentrHeavin(int** matrix, int size);
int sum(int* arreyDistance, int size);
int getMin(int* Arr, int size);