#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct tnode* add_element(int x, tnode* tree);
void print_tree(struct tnode* tree);
void find(struct tnode* tree, int element);

int main()
{
    int start = 1;
    int data = 0, element = 9;

    struct tnode* root = NULL;
    printf("-1 - end\n");
    while (start)
    {
        printf("Enter number: ");
        scanf("%d", &data);
        if (data == -1)
        {
            printf("end tree\n");
            start = 0;
        }
        else
            root = add_element(data, root);
    }

    print_tree(root);
    find(root, element);

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

void print_tree(struct tnode* tree)
{
    if (tree != NULL) {
        print_tree(tree->left);
        printf("%d \n", tree->data);
        print_tree(tree->right);
    }
}

void find(struct tnode* tree, int element) 
{
    if (tree != NULL) 
    {
        find(tree->left, element);
        if (tree->data == element)
            printf("Element %d found\n", element);
        find(tree->right, element);
    }
}