#include <math.h>
#include <stdbool.h>
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
void inOrder(Tree *avl);
void preOrder(Tree *avl);
void postOrder(Tree *avl);
Tree *balacing(Tree *child, Tree *parent);
void showTree(Tree *avl);
Tree *loadTreeFromFile(Tree *arvore, char *file_name);
int searchValue(Tree *avl, Tree **node, int value, int nivel);
Tree *removeValue(Tree *avl, int value);
Tree *sweepTreeFix(Tree *avl, int *signal_balacing);

int main()
{
    Tree *avl = NULL;
    char nome_arquivo[10];
    int full = 1, nivel_base = 0, nivel = 0, op = 0;

    printf("Escolha um arquivo (presente na pasta avl_scripts): ");
    scanf("%s", nome_arquivo);
    avl = loadTreeFromFile(avl, nome_arquivo);

    do
    {
        printf("________________________________________________________________________________\n");
        printf("1 - Mostrar Arvore\n");
        printf("2 - Buscar No na Arvore\n");
        printf("3 - Altura da Arvore\n");
        printf("4 - Remover No da Arvore\n");
        printf("5 - Adicionar No na arvore\n");
        printf("6 - Print In Order\n");
        printf("7 - Print Pre Order\n");
        printf("8 - Print Post Order\n");
        printf("0- Sair\n");
        printf("________________________________________________________________________________\n");
        scanf("%d", &op);
        getchar();

        if (op == 1)
        {
            printf("Arvore:\n");
            showTree(avl);
        }
        else if (op == 2)
        {
            int valor = 0;
            Tree *node = NULL;
            printf("No para buscar: ");
            scanf("%d", &valor);
            getchar();

            nivel = searchValue(avl, &node, valor, nivel_base);
            if (nivel == 0)
            {
                printf("Valor não Encontrado\n");
            }
            else
            {
                printf("Valor se encontra no nível: %d\n", nivel);
                if (node->parent != NULL)
                    printf("Valor do pai: %d\n", node->parent->value);
                if (node->left_child != NULL)
                    printf("Valor do filho esq: %d\n", node->left_child->value);
                if (node->right_child != NULL)
                    printf("Valor do filho dir: %d\n", node->right_child->value);
            }
        }
        else if (op == 3)
        {
            int height = 0;
            height = (avl->left_height > avl->right_height) ? avl->left_height : avl->right_height;
            printf("Altura da arvore: %d\n", height);
        }
        else if (op == 4)
        {
            int valor = 0;
            Tree *aux = NULL;
            int signal = 0; // verifica se ouve um balanceamento
            printf("Valor do No a ser excluido: ");
            scanf("%d", &valor);
            getchar();
            aux = removeValue(avl, valor);
            if (aux->parent == NULL)
                avl = aux;
            printf("Arvore depois da remocao\n");
            showTree(avl);
            // verifica e corrige a arvore
            avl = sweepTreeFix(avl, &signal);
            if (signal == 1)
            {
                printf("Depois do balanceamento necessario\n");
                showTree(avl);
            }
        }
        else if (op == 5)
        {
            int value;
            printf("Digite algum numero para o indice do no: ");
            scanf("%d", &value);
            avl = addNode(avl, value);
        }
        else if (op == 6)
        {
            printf("PRINT IN ORDER:\n");
            inOrder(avl);
            printf("\n");
        }
        else if (op == 7)
        {
            printf("PRINT PRE ORDER:\n");
            preOrder(avl);
            printf("\n");
        }
        else if (op == 8)
        {
            printf("PRINT POST ORDER\n");
            postOrder(avl);
            printf("\n");
        }

        if (op != 0)
            getchar();
        printf("\e[H\e[2J"); // limpa tela
    } while (op != 0);

    return 0;
}

Tree *loadTreeFromFile(Tree *avl, char *file_name)
{
    FILE *file;
    int value;
    char *path = (char *)malloc(30 * sizeof(char));
    sprintf(path, "./avl_scripts/%s.txt", file_name);

    file = fopen(path, "r");

    if (file == NULL)
    {
        printf("arquivo nao encontrado...\n");
        exit(-1);
    }
    else
    {
        while (fscanf(file, "%d", &value) != EOF)
        {
            avl = addNode(avl, value);
        }
    }
    return avl;
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
                aux = balacing(new_node, aux);
                // se o pai do node que foi balanceado eh nulo,
                // entao a raiz da avl eh atualizada
                if (aux->parent == NULL)
                {
                    avl = aux;
                }
            }
        }
        else
        {
            aux_child = aux->left_child;
            if (aux->left_child == NULL)
            {
                aux->left_child = new_node;
                new_node->parent = aux;
                aux = balacing(new_node, aux);
                if (aux->parent == NULL)
                {
                    avl = aux;
                }
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

Tree *balacing(Tree *child, Tree *parent)
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
        return parent;
    }

    if (parent->parent != NULL)
        parent = balacing(parent, parent->parent);
    return parent;
}

void preOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    printf("%d, ", avl->value);
    preOrder(avl->left_child);
    preOrder(avl->right_child);
}

void inOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    inOrder(avl->left_child);
    printf("%d, ", avl->value);
    inOrder(avl->right_child);
}

void postOrder(Tree *avl)
{
    if (avl == NULL)
        return;
    postOrder(avl->left_child);
    printf("%d, ", avl->value);
    postOrder(avl->right_child);
}

int showTreeFormated(Tree *avl, int is_left, int offset, int depth, char coordinate[20][255])
{
    char val[20];
    int larg = 5;

    if (!avl)
        return 0;

    sprintf(val, "[%03d]", avl->value);

    int left = showTreeFormated(avl->left_child, 1, offset, depth + 1, coordinate);
    int right = showTreeFormated(avl->right_child, 0, offset + left + larg, depth + 1, coordinate);
    for (int i = 0; i < larg; i++)
        coordinate[2 * depth][offset + left + i] = val[i];

    if (depth && is_left)
    {

        for (int i = 0; i < larg + right; i++)
            coordinate[2 * depth - 1][offset + left + larg / 2 + i] = '-';

        coordinate[2 * depth - 1][offset + left + larg / 2] = '*';
        coordinate[2 * depth - 1][offset + left + larg + right + larg / 2] = '*';
    }
    else if (depth && !is_left)
    {

        for (int i = 0; i < left + larg; i++)
            coordinate[2 * depth - 1][offset - larg / 2 + i] = '-';

        coordinate[2 * depth - 1][offset + left + larg / 2] = '*';
        coordinate[2 * depth - 1][offset - larg / 2 - 1] = '*';
    }

    return left + larg + right;
}

void showTree(Tree *avl)
{
    char ponto[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(ponto[i], "%100s", " ");

    showTreeFormated(avl, 0, 0, 0, ponto);

    for (int i = 0; i < 20; i++)
        printf("%s\n", ponto[i]);
}

int searchValue(Tree *avl, Tree **node, int value, int nivel)
{
    if (avl != NULL)
    {
        if (value > avl->value)
        {
            nivel += 1;
            searchValue(avl->right_child, node, value, nivel);
        }
        else if (value < avl->value)
        {
            nivel += 1;
            searchValue(avl->left_child, node, value, nivel);
        }
        else
        {
            // valor == arvore->valor
            nivel++;
            *node = avl; // nó encontrado
            return nivel;
        }
    }
    else
    {
        return 0;
    }
}

Tree *removeValue(Tree *avl, int value)
{
    if (avl != NULL)
    {
        if (value > avl->value)
        {
            avl->right_child = removeValue(avl->right_child, value);
        }
        else if (value < avl->value)
        {
            avl->left_child = removeValue(avl->left_child, value);
        }
        else if (value == avl->value)
        {
            // value a ser removido, encontrado
            if (avl->left_child != NULL && avl->right_child != NULL)
            { // o no possui dois filhos, deve procurar o sucessor mais proximo
                Tree *avl_aux;
                avl_aux = avl->right_child;
                if (avl_aux->left_child == NULL)
                {
                    if (avl->left_child != NULL)
                        avl_aux->left_child = avl->left_child;
                    else
                        avl_aux->left_child = NULL;
                    avl_aux->parent->right_child = NULL;
                    if (avl->parent != NULL)
                    {
                        avl_aux->parent = avl->parent;
                        if (avl->value > avl->parent->value)
                        {
                            avl->parent->right_child = avl_aux;
                        }
                        else if (avl->value < avl->parent->value)
                        {
                            avl->parent->left_child = avl_aux;
                        }
                    }
                    else
                    {
                        avl_aux->parent = NULL;
                    }
                    free(avl);

                    return avl_aux;
                }
                else
                {
                    while (!(avl_aux->left_child == NULL && avl_aux->right_child == NULL))
                    {
                        avl_aux = avl_aux->left_child;
                        if (avl_aux->left_child == NULL)
                        {
                            Tree *old_parent_avl_aux = avl_aux->parent;
                            // o no é o sucessor do numero removido
                            avl_aux->left_child = avl->left_child;
                            // passa o filho direito do aux para o pai
                            old_parent_avl_aux->left_child = avl_aux->right_child;
                            avl_aux->right_child = avl->right_child;
                            if (avl->parent != NULL)
                            {
                                avl_aux->parent = avl->parent;
                                if (avl_aux->value > avl->parent->value)
                                {
                                    avl->parent->right_child = avl_aux;
                                }
                                else if (avl_aux->value < avl->parent->value)
                                {
                                    avl->parent->left_child = avl_aux;
                                }
                            }
                            else
                            {
                                avl_aux->parent = NULL;
                            }
                            free(avl);
                            return avl_aux;
                        }
                    }
                }
            }
            else
            {
                if (avl->parent != NULL)
                {
                    if (value > avl->parent->value)
                    {
                        if (avl->right_child != NULL && avl->left_child == NULL)
                            avl->parent->right_child = avl->right_child;
                        else if (avl->left_child != NULL && avl->right_child == NULL)
                            avl->parent->right_child = avl->left_child;
                        else
                            avl->parent->right_child = NULL;
                        free(avl);
                        return avl->parent->right_child;
                    }
                    else if (value < avl->parent->value)
                    {
                        if (avl->right_child != NULL && avl->left_child == NULL)
                            avl->parent->left_child = avl->right_child;
                        else if (avl->left_child != NULL && avl->right_child == NULL)
                            avl->parent->left_child = avl->left_child;
                        else
                            avl->parent->left_child = NULL;
                        free(avl);
                        return avl->parent->left_child;
                    }
                }
                else
                {
                    if (avl->left_child != NULL && avl->right_child == NULL)
                    {
                        avl->left_child->parent = NULL;
                        free(avl);
                        return avl->left_child;
                    }
                    else if (avl->right_child != NULL && avl->left_child == NULL)
                    {
                        avl->right_child->parent = NULL;
                        free(avl);
                        return avl->right_child;
                    }
                    else
                    {
                        avl = NULL;
                        free(avl);
                        return NULL;
                    }
                }
            }
        }
    }
    else
    {
        printf("Valor não encontrado\n");
        return avl;
    }
}

Tree *postOrderFix(Tree *avl, int *signal_balacing)
{
    // realiza varredura de baixo da arvore ate a raiz
    // conserta as alturas e realiza rotacoes quando necessario
    if (avl == NULL)
        return NULL;
    postOrderFix(avl->left_child, signal_balacing);
    postOrderFix(avl->right_child, signal_balacing);

    fixHeight(avl);

    // verifica o balanceamento e realiza as rotacoes necessarias
    Tree *parent = avl, *child;
    int balancing_factor_p = parent->right_height - parent->left_height;
    int balancing_factor_c;

    if (balancing_factor_p <= -2)
    {
        *signal_balacing = 1;
        child = parent->left_child;
        balancing_factor_c = child->right_height - child->left_height;
        if (balancing_factor_c >= 1)
            parent->left_child = leftRotate(child);
        Tree *grand_parent = parent->parent;
        parent = rightRotate(parent);
        return parent;
    }
    else if (balancing_factor_p >= 2)
    {
        *signal_balacing = 1;
        child = parent->right_child;
        balancing_factor_c = child->right_height - child->left_height;
        if (balancing_factor_c <= -1)
        {
            parent->right_child = rightRotate(child);
        }
        parent = leftRotate(parent);
        if (parent->parent != NULL)
            return parent;
    }
    return parent;
}

Tree *sweepTreeFix(Tree *avl, int *signal_balacing)
{
    return postOrderFix(avl, signal_balacing);
}