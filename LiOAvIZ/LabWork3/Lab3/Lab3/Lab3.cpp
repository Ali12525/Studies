#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void push();
void review();
void pop();
void del(int priority);
struct node* create_node();     
struct node* create_node_list(int priority, char* task);
void add_element(int priority, char* task);
void push_logic(struct node* p);
char* take_element();

struct node* head = NULL;

int main()
{
	char ch;
	int ext = 0, extt = 0, priority = -1;

	do
	{
		system("cls");
		printf("1. Add element\n2. Take element\n3. View list\n4. Delete element\n");
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
			printf("Press any key to exit");
			getchar();
			break;
		case '3':
			review();
			printf("Press any key to exit");
			getchar();
			break;
		case '4':
			do
			{
				printf("Enter the priority: ");
				scanf("%d", &priority);
				if (priority == -1)
				{
					printf("no recording was made\n");
					scanf("%*[^\n]");
					getchar();
					continue;
				}
				getchar();
				extt = 1;
			} while (extt == 0);
			del(priority);
			printf("Press any key to exit");
			getchar();
			break;
		default:
			break;
		}
	} while (ext == 0);

    return 0;
}

struct node
{
	int priority;
	char task[50];
	struct node* next;
};

struct node* create_node()
{
	struct node* p = NULL;
	int priority = -1;
	char task[50];
	int ext = 0;

	if ((p = (node*)malloc(sizeof(struct node))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	do
	{
		printf("Priority input: ");
		scanf("%d", &priority);
		if (priority == -1)
		{
			printf("no recording was made\n");
			scanf("%*[^\n]");
			getchar();
			continue;
		}
		getchar();
		printf("Enter a task: ");
		fgets(task, 50, stdin);
		
		if (*task == 0)
		{
			printf("no recording was made\n");
			scanf("%*[^\n]");
			getchar();
			continue;
		}
		ext = 1;
	} while (ext == 0);

	p->priority = priority;
	strcpy(p->task, task);
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
		printf("Priority: %d, \n", struc->priority);
		printf("Task: %s", struc->task);
		struc = struc->next;
	}
}

void del(int priority)
{
	struct node* struc = head;
	struct node* prev = struc;
	int flag = 0;

	if (head == NULL)
	{
		printf("the list is empty\n");
		return;
	}

	if (struc->priority == priority)
	{
		flag = 1;
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
		if (struc->priority == priority)
		{
			flag = 1;
			if (struc->next)
			{
				prev->next = struc->next;
				free(struc);
				struc = prev->next;
			}
			else
			{
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
		printf("element not found\n");
		return;
	}
}

void push()
{
	struct node* p = NULL;
	p = create_node();
	push_logic(p);
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
	printf("Priority: %d, \n", struc->priority);
	printf("Task: %s", struc->task);
	free(struc);
}

char* take_element()
{
	if (head == NULL)
	{
		printf("the list is empty\n");
		return NULL;
	}
	char* data = (char*)malloc(50 * sizeof(char));
	struct node* struc = head;
	head = struc->next;
	strcpy(data, struc->task);
	free(struc);
	return data;
}

struct node* create_node_list(int priority, char* task)
{
	struct node* p = NULL;

	if ((p = (node*)malloc(sizeof(struct node))) == NULL)
	{
		printf("memory allocation error\n");
		exit(1);
	}

	p->priority = priority;
	strcpy(p->task, task);
	p->next = NULL;

	return p;
}

void add_element(int priority, char* task)
{
	struct node* p = NULL;
	p = create_node_list(priority, task);
	push_logic(p);
}

void push_logic(struct node* p)
{
	if (head == NULL && p != NULL)
	{
		head = p;
		return;
	}
	else
	{
		struct node* struc = head;
		struct node* prev;

		if (p->priority > struc->priority)
		{
			prev = struc->next;
			p->next = struc;
			struc->next = prev;
			head = p;
			return;
		}

		while (struc)
		{
			prev = struc;
			struc = struc->next;
			if (struc)
			{
				if (p->priority < prev->priority && p->priority > struc->priority)
				{
					prev->next = p;
					p->next = struc;
					break;
				}
			}
			else
			{
				struc = prev;
				if (p->priority > struc->priority)
				{
					p->next = struc;
					struc->next = NULL;
				}
				else
				{
					struc->next = p;
					p->next = NULL;
				}
				break;
			}
		}
	}
}