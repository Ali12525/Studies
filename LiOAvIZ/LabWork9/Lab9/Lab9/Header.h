#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void fill_arr(int** Arr, int size);
void print_arr(int** Arr, int size);
int find_arr(int* Arr, int size, int element);
struct node* createNode(int v);
struct Graph* createGraph(int vertices);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
struct Graph* transformGraph(int** Arr, int size);
int BfsList(struct Graph* graph, int size, int vertex, int end);
int Bfs(int** Matrix, int size, int vertex, int end, int shutdown);
void autoTest(int startVertex, int endVertex);
int* fill_visited(int* visited, int size);

int DFS(int** Matrix, int size, int start, int end, int shutdown);
void Dfs(int** Matrix, int size, int vertex, int* visited, int end, int* c, int shutdown);
int DFSList(struct Graph* graph, int size, int start, int end);
void DfsList(struct Graph* graph, int vertex, int* visited, int end, int* c);