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
void DFS(int** Matrix, int size);
void Dfs(int** Matrix, int size, int vertex, int* visited);
void DFSList(struct Graph* graph, int size);
void DfsList(struct Graph* graph, int vertex, int* visited);
void iterativeDFS(int** Matrix, int size);
void iterativeDfs(int** Matrix, int size, int vertex, int* visited);

struct nodeStack* create_node(int inf);
int pop();
void push(int inf);
int stackEmpty();