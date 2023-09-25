#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void push();
void review();
void del(char* name);
struct node* create_node();
void find(char* name);
void pop();
struct node* create_node_list(char* inf);
void take_element();
void add_element(char* inf);

struct node* head = NULL;

int main()
{
	char ch;
	int exit = 0;
	char inf[256];

	do
	{
		system("cls");
		printf("1. Add element\n2. Take element\n3. View list\n4. Find element\n5. Delete element\n");
		printf("Select item: ");
		scanf("%c", &ch);
		system("cls");
		if (ch != '\n')
		{
			scanf("%*[^\n]");
			getchar();
		}

		switch (ch)
		{
		case '1':
			push();
			break;
		case '2':
			pop();
			getchar();
			break;
		case '3':
			review();
			getchar();
			break;
		case '4':
			printf("Enter the element name: ");
			fgets(inf, 256, stdin);
			find(inf);
			printf("Press any key to exit");
			getchar();
			break;
		case '5':
			printf("Enter the element name: ");
			fgets(inf, 256, stdin);
			del(inf);
			printf("Press any key to exit");
			getchar();
			break;
		default:
			break;
		}
	} while (exit == 0);

	return 0;
}

struct node
{
	char inf[256];
	struct node* next;
};

struct node* create_node()
{
	struct node* p = NULL;
	char inf[256];
	int ext = 0;

	if ((p = (node*)malloc(sizeof(struct node))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	do
	{
		printf("Enter a task: ");
		fgets(inf, 256, stdin);

		if (*inf == 0)
		{
			printf("no recording was made\n");
			scanf("%*[^\n]");
			getchar();
			continue;
		}
		ext = 1;
	} while (ext == 0);

	strcpy(p->inf, inf);
	p->next = NULL;

	return p;
}

void review()
{
	struct node* struc = head;
	if (head == NULL)
	{
		printf("the list is empty\n");
	}
	while (struc)
	{
		printf("Task: %s", struc->inf);
		struc = struc->next;
	}
	printf("Press any key to exit");
}

void push()
{
	struct node* p = NULL;
	p = create_node();
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

void find(char* name)
{
	struct node* struc = head;
	if (head == NULL)
	{
		printf("the list is empty\n");
	}
	while (struc)
	{
		if (strcmp(name, struc->inf) == 0)
		{
			printf("Element found");
			printf("\nInfo: %s", struc->inf);
			return;
		}
		struc = struc->next;
	}
	printf("Element not found\n");
	return;
}

void del(char* name)
{
	struct node* struc = head;
	struct node* prev = struc;
	int flag = 0;

	if (head == NULL)
	{
		printf("the list is empty\n");
		return;
	}

	if (strcmp(name, struc->inf) == 0)
	{
		flag = 1;
		printf("Successful element deletion %s", struc->inf);
		head = struc->next;
		free(struc);
		struc = head;
	}
	else
	{
		prev = struc;
		struc = struc->next;
	}

	while (struc)
	{
		if (strcmp(name, struc->inf) == 0)
		{
			flag = 1;
			if (struc->next)
			{
				printf("Successful element deletion %s", struc->inf);
				prev->next = struc->next;
				free(struc);
				struc = prev->next;
			}
			else
			{
				printf("Successful element deletion %s", struc->inf);
				prev->next = NULL;
				free(struc);
				return;
			}
		}
		else
		{
			prev = struc;
			struc = struc->next;
		}
	}

	if (flag == 0)
	{
		printf("Element not found\n");
		return;
	}
}


void pop()
{
	if (head == NULL)
	{
		printf("the list is empty\n");
		return;
	}
	struct node* struc = head;
	head = struc->next;
	printf("Info: %s", struc->inf);
	free(struc);
	printf("Press any key to exit");
}

void add_element(char* inf)
{
	struct node* p = NULL;
	p = create_node_list(inf);
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

void take_element()
{
	if (head == NULL)
	{
		printf("the list is empty\n");
		return;
	}
	struct node* struc = head;
	head = struc->next;
	free(struc);
}

struct node* create_node_list(char* inf)
{
	struct node* p = NULL;
	int ext = 0;

	if ((p = (node*)malloc(sizeof(struct node))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	strcpy(p->inf, inf);
	p->next = NULL;

	return p;
}