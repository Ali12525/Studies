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
void BFSList(struct Graph* graph, int size, int vertex);
void BfsList(struct Graph* graph, int vertex, int* visited);
void BFS(int** Matrix, int size, int shutdown, int vertex);
void Bfs(int** Matrix, int size, int vertex, int* visited, int shutdown);
void noClassBFS(int** Matrix, int size, int shutdown, int vertex);
void noClassBfs(int** Matrix, int size, int vertex, int* visited, int shutdown);
void autoTest(int vertex);

struct nodeQueue* create_node(int inf);
int pop();
void push(int inf);
int queueEmpty();