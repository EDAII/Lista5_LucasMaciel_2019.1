#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
Tree *Balacing(Tree *child, Tree *parent);

int main()
{
    int array[] = {20, 33, 5, 9, 3, 2, 56, 71, 15, 10, 16, 1};
    Tree *avl = NULL;
    int size = sizeof(array) / sizeof(int);
    for (int i = 0; i < size; i++)
    {
        avl = addNode(avl, array[i]);
    }

    printf("InOrder: ");
    inOrder(avl);
    printf("\n");

    printf("PreOrder:\n");
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
                aux = Balacing(new_node, aux);
                // se o pai do node que foi balanceado eh nulo,
                // entao a raiz da arvore eh atualizada
                if (aux->parent == NULL)
                    avl = aux;
            }
        }
        else
        {
            aux_child = aux->left_child;
            if (aux->left_child == NULL)
            {
                aux->left_child = new_node;
                new_node->parent = aux;
                aux = Balacing(new_node, aux);
                if (aux->parent == NULL)
                    avl = aux;
            }
        }
    }
    return avl;
}

void fixHeight(Tree *child)
{
    int left_h, right_h;
    // consertar alturas do filho
    if (child->left_child != NULL)
    {
        left_h = child->left_child->left_height;
        right_h = child->left_child->right_height;
        // somando 1 a maior altura dos filhos da rotacao
        child->left_height = 1 + ((left_h >= right_h) ? left_h : right_h);
    }
    else
    {
        child->left_height = 0;
    }
    // lado direito do filho
    if (child->right_child != NULL)
    {
        left_h = child->right_child->left_height;
        right_h = child->right_child->right_height;
        child->right_height = 1 + ((left_h >= right_h) ? left_h : right_h);
    }
    else
    {
        child->right_height = 0;
    }

    // // consertar alturas do pai
    // left_h = (parent->left_child != NULL) ? parent->left_child->left_height : 0;
    // right_h = (parent->left_child != NULL) ? parent->left_child->right_height : 0;
    // parent->left_height = 1 + ((left_h >= right_h) ? left_h : right_h);
    // left_h = (parent->right_child != NULL) ? parent->right_child->left_height : 0;
    // right_h = (parent->right_child != NULL) ? parent->right_child->right_height : 0;
    // parent->right_height = 1 + ((left_h >= right_h) ? left_h : right_h);
}

Tree *leftRotate(Tree *parent)
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
    {
        if (new_parent->value > grand_parent->value)
            grand_parent->right_child = new_parent;
        else
            grand_parent->left_child = new_parent;
    }
    fixHeight(parent);
    fixHeight(new_parent);
    // retorna o novo pai
    return new_parent;
}

Tree *rightRotate(Tree *parent)
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
    {
        if (new_parent->value > grand_parent->value)
            grand_parent->right_child = new_parent;
        else
            grand_parent->left_child = new_parent;
    }
    fixHeight(parent);
    fixHeight(new_parent);
    // retorna o novo pai
    return new_parent;
}

Tree *Balacing(Tree *child, Tree *parent)
{
    if (child->value > parent->value)
        parent->right_height++;
    else
        parent->left_height++;

    if (parent->right_height == parent->left_height)
        return parent;
    int balancing_factor_p = parent->right_height - parent->left_height;
    int balancing_factor_c = child->right_height - child->left_height;

    if (balancing_factor_p <= -2)
    {
        if (balancing_factor_c >= 1)
            parent->left_child = leftRotate(child);
        Tree *grand_parent = parent->parent;
        parent = rightRotate(parent);
        return parent;
    }
    else if (balancing_factor_p >= 2)
    {
        if (balancing_factor_c <= -1)
        {
            parent->right_child = rightRotate(child);
        }
        parent = leftRotate(parent);
        if (parent->parent != NULL)
            return parent;
    }

    if (parent->parent != NULL)
        parent = Balacing(parent, parent->parent);
    return parent;
}

void preOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    printf("%d: l%d, r%d \n", avl->value, avl->left_height, avl->right_height);
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