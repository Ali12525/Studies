#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct tnode* add_element(int x, tnode* tree);
struct tnode* add_unequal_element(int x, tnode* tree);
void print_tree(struct tnode* r, int l);
void view_tree(struct tnode* tree);
void find(struct tnode* tree, int element);
void total(struct tnode* tree, int element);
void total_amount(struct tnode* tree);
void occurrence(struct tnode* tree, int element);

int count = 0;

int main()
{
	int start = 1;
	int data = 0, element = 9;
	char ch;
	int ext = 0;
	struct tnode* root = NULL;

	do
	{
		system("cls");
		printf("1. Add element\n2. Adding unequal elements\n3. Print tree\n4. Find element\n5. Occurrence of the element\n6. View tree\n7. Amount of the element\n");
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
			printf("Enter number: ");
			scanf("%d", &data);
			root = add_element(data, root);
			break;
		case '2':
			printf("Enter number: ");
			scanf("%d", &data);
			root = add_unequal_element(data, root);
			printf("\nPress any key to exit");
			if (count > 0)
			{
				count = 0;
				getchar();
				getchar();
			}
			break;
		case '3':
			printf("Tree: \n");
			print_tree(root, 6);
			printf("\nPress any key to exit");
			getchar();
			break;
		case '4':
			printf("Enter element: ");
			scanf("%d", &element);
			find(root, element);
			total(root, element);
			if (count == 0)
				printf("Element not found\n");
			count = 0;
			printf("Press any key to exit");
			getchar();
			getchar();
			break;
		case '5':
			printf("Enter element: ");
			scanf("%d", &element);
			occurrence(root, element);
			printf("Occurrence: %d", count);
			count = 0;
			printf("\nPress any key to exit");
			getchar();
			getchar();
			break;
		case '6':
			printf("Tree: \n");
			view_tree(root);
			printf("\nPress any key to exit");
			getchar();
			break;
		case '7':
			count = 0;	
			total_amount(root);
			printf("Amount: %d", count++);
			count = 0;
			printf("\nPress any key to exit");
			getchar();
			break;
		default:
			break;
		}
	} while (ext == 0);

	return 0;
}

struct tnode
{
	int data;
	struct tnode* left;
	struct tnode* right;
};

struct tnode* add_element(int x, tnode* tree)
{
	if (tree == NULL)
	{
		tree = (struct tnode*)malloc(sizeof(struct tnode));
		tree->data = x;
		tree->left = NULL;
		tree->right = NULL;
	}
	else  if (x < tree->data)
		tree->left = add_element(x, tree->left);
	else
		tree->right = add_element(x, tree->right);
	return(tree);
}

struct tnode* add_unequal_element(int x, tnode* tree)
{
	total(tree, x);
	if (count > 0)
	{
		printf("Such an element already exists");
		return(tree);
	}

	if (tree == NULL)
	{
		tree = (struct tnode*)malloc(sizeof(struct tnode));
		tree->data = x;
		tree->left = NULL;
		tree->right = NULL;
	}
	else  if (x < tree->data)
		tree->left = add_unequal_element(x, tree->left);
	else
		tree->right = add_unequal_element(x, tree->right);
	return(tree);
}

void print_tree(struct tnode* r, int l)
{

	if (r == NULL)
	{
		return;
	}

	print_tree(r->right, l + 1);
	for (int i = 0; i < l; i++)
	{
		printf(" ");
	}

	printf("%d  \n", r->data);
	print_tree(r->left, l + 1);
}

void view_tree(struct tnode* tree)
{
	if (tree != NULL) {
		printf("%d ", tree->data);
		count++;
		view_tree(tree->left);
		view_tree(tree->right);
	}
}

void find(struct tnode* tree, int element)
{
	if (tree != NULL)
	{
		if(element < tree->data)
			find(tree->left, element);
		if (tree->data == element)
			printf("Element %d found\n", element);
		if (element > tree->data)
			find(tree->right, element);
	}
}

void total(struct tnode* tree, int element)
{
	if (tree != NULL)
	{
		if (element <= tree->data)
			total(tree->left, element);
		if (tree->data == element)
			count++;
		if (element >= tree->data)
			total(tree->right, element);
	}
}

void total_amount(struct tnode* tree)
{
	if (tree != NULL) {
		
		count++;
		total_amount(tree->left);
		total_amount(tree->right);
	}
}

void occurrence(struct tnode* tree, int element)
{
	if (tree != NULL)
	{
		if (element < tree->data)
			occurrence(tree->left, element);
		count++;
		if (element > tree->data)
			occurrence(tree->right, element);
	}
}

