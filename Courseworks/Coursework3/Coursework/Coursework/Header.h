#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string>

struct node* createNode(int v);
struct Graph* createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
int dfs(struct Graph* graph, int vertex, int* visited, int* pair);
void fillGraphAuto(struct Graph* graph, int sizeFirstShare, int sizeSecondShare);
void fillGraphManually(struct Graph* graph, int sizeFirstShare, int sizeSecondShare);
int total(struct Graph* graph, int src, int vertex);
void printKun(int* arr, int size);
int* fillArrey(int* arr, int size, int x);
void freeGraph(struct Graph* graph, int size);
void kuhnAlgorithm(struct Graph* graph, int* pair, int sizeFirstShare, int sizeSecondShare);

struct node
{
	int vertex;
	struct node* next;
};

struct Graph
{
	int numVertices;
	struct node** adjLists;
};