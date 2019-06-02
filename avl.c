#include <stdio.h>
#include <stdlib.h>

struct tree
{
    int value;
    int left_height;
    int right_height;
    struct tree *right_child;
    struct tree *left_child;
    struct tree *parent;
} typedef Tree;

Tree *addNode(Tree *avl, int value);
void preOrder(Tree *avl);
void inOrder(Tree *avl);
void Balacing(Tree *child, Tree *parent);

int main()
{
    int array[] = {20, 33, 5, 9, 3, 2, 56, 71, 1};
    Tree *avl = NULL;

    for (int i = 0; i < 9; i++)
        avl = addNode(avl, array[i]);

    printf("InOrder: ");
    inOrder(avl);
    printf("\n");

    printf("PreOrder: ");
    preOrder(avl);
    printf("\n");

    return 0;
}

Tree *addNode(Tree *avl, int value)
{
    Tree *new_node;
    Tree *aux, *aux_child;
    new_node = (Tree *)malloc(sizeof(Tree));

    new_node->value = value;
    new_node->right_height = 0;
    new_node->left_height = 0;
    new_node->right_child = NULL;
    new_node->left_child = NULL;

    if (avl == NULL)
    {
        new_node->parent = NULL;
        return new_node;
    }

    for (aux = avl; aux != NULL; aux = aux_child)
    {
        if (value > aux->value)
        {
            aux_child = aux->right_child;
            if (aux->right_child == NULL)
            {
                // adiciona no
                aux->right_child = new_node;
                new_node->parent = aux;
                Balacing(new_node, aux);
            }
        }
        else
        {
            aux_child = aux->left_child;
            if (aux->left_child == NULL)
            {
                aux->left_child = new_node;
                new_node->parent = aux;
                Balacing(new_node, aux);
            }
        }
    }
    return avl;
}

void fixHeight(Tree *child, Tree *parent)
{
    // consertar alturas do filho
    int l_h = (child->left_child) ? child->left_child->left_height : 0;
    int r_h = (child->left_child) ? child->left_child->right_height : 0;
    // somando 1 a maior altura dos filhos da rotacao
    child->left_height = (l_h != 0 || r_h != 0) ? (1 + ((l_h >= r_h) ? l_h : r_h)) : 0;
    // lado direito do filho
    l_h = (child->right_child) ? child->right_child->left_height : 0;
    r_h = (child->right_child) ? child->right_child->right_height : 0;
    child->right_height = (l_h != 0 || r_h != 0) ? (1 + ((l_h >= r_h) ? l_h : r_h)) : 0;

    // consertar alturas do pai
    l_h = (parent->left_child) ? parent->left_child->left_height : 0;
    r_h = (parent->left_child) ? parent->left_child->right_height : 0;
    parent->left_height = (l_h != 0 || r_h != 0) ? (1 + ((l_h >= r_h) ? l_h : r_h)) : 0;
    l_h = (parent->right_child) ? parent->right_child->left_height : 0;
    r_h = (parent->right_child) ? parent->right_child->right_height : 0;
    parent->right_height = (l_h != 0 || r_h != 0) ? (1 + ((l_h >= r_h) ? l_h : r_h)) : 0;
}

void leftRotate(Tree *parent)
{
    Tree *grand_parent = parent->parent;
    Tree *new_parent = parent->right_child;
    Tree *new_child = new_parent->left_child;

    new_parent->left_child = parent;
    new_parent->parent = grand_parent;
    parent->right_child = new_child;
    parent->parent = new_parent;
    if (new_child != NULL)
        new_child->parent = parent;
    if (grand_parent != NULL)
        grand_parent->right_child = new_parent;
    fixHeight(parent, new_parent);
}

void rightRotate(Tree *parent)
{
    Tree *grand_parent = parent->parent;
    Tree *new_parent = parent->left_child;
    Tree *new_child = new_parent->right_child;

    new_parent->right_child = parent;
    new_parent->parent = grand_parent;
    parent->left_child = new_child;
    parent->parent = new_parent;
    if (new_child != NULL)
        new_child->parent = parent;
    if (grand_parent != NULL)
        grand_parent->left_child = new_parent;
    fixHeight(parent, new_parent);
}

void Balacing(Tree *child, Tree *parent)
{
    if (child->value > parent->value)
        parent->right_height++;
    else
        parent->left_height++;

    if (parent->right_height == parent->left_height)
        return;
    int balancing_factor_p = parent->right_height - parent->left_height;
    int balancing_factor_c = child->right_height - child->left_height;

    if (balancing_factor_p <= -2)
    {
        if (balancing_factor_c >= 1)
            leftRotate(child);
        rightRotate(parent);
        return;
    }
    else if (balancing_factor_p >= 2)
    {
        if (balancing_factor_c <= -1)
            rightRotate(child);
        leftRotate(parent);
        return;
    }

    if (parent->parent != NULL)
        Balacing(parent, parent->parent);
    return;
}

void preOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    printf("%d ", avl->value);
    preOrder(avl->left_child);
    preOrder(avl->right_child);
}

void inOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    inOrder(avl->left_child);
    printf("%d ", avl->value);
    inOrder(avl->right_child);
}