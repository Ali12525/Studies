#include "Header.h"

struct nodeQueue* head = NULL, * last = NULL;

struct nodeQueue	
{
	int inf;
	struct nodeQueue* next;
};

void push(int inf)
{
	struct nodeQueue* p = NULL;
	p = create_node(inf);
	if (head == NULL && p != NULL)
	{
		head = p;
		last = p;
	}
	else if (head != NULL && p != NULL)
	{
		last->next = p;
		last = p;
	}
	return;
}

int pop()
{
	if (head == NULL)
	{
		printf("the list is empty\n");
		return NULL;
	}
	struct nodeQueue* struc = head;
	head = struc->next;
	int data = struc->inf;
	free(struc);

	return data;
}

struct nodeQueue* create_node(int inf)
{
	struct nodeQueue* p = NULL;
	int ext = 0;

	if ((p = (nodeQueue*)malloc(sizeof(struct nodeQueue))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	p->inf = inf;
	p->next = NULL;

	return p;
}

int queueEmpty()
{
	if (head == NULL)
		return 1;
	else
		return 0;
}