#include "Header.h"

struct nodeStack* head = NULL;

struct nodeStack
{
	int inf;
	struct nodeStack* next;
};

void push(int inf)
{
	struct nodeStack* p = NULL;
	p = create_node(inf);
	if (head == NULL && p != NULL)
	{
		head = p;
	}

	else if (head != NULL && p != NULL)
	{
		p->next = head;
		head = p;
	}
	return;
}

int pop()
{
	if (head == NULL)
	{
		return -1;
	}
	int data = 0;
	struct nodeStack* struc = head;
	head = struc->next;
	data = struc->inf;
	free(struc);
	return data;
}

int stackEmpty()
{
	if (head == NULL)
		return 1;
	else
		return 0;
}

struct nodeStack* create_node(int inf)
{
	struct nodeStack* p = NULL;
	int ext = 0;

	if ((p = (nodeStack*)malloc(sizeof(struct nodeStack))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	p->inf = inf;
	p->next = NULL;

	return p;
}