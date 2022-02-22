#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char word[50];
    struct Node *left, *right, *parent;
    int depth;
} Node;

typedef struct Tree
{
    Node *root;
} Tree;

Node* createNode(char *word);
int max(int a, int b);
void addNode(Tree *avl, Node *new_node);
void search(Tree *avl, char *word);
Node* getSuccessor(Node *node);
Node* getPredecessor(Node *node);
Node* rotateLeft(Tree *avl, Node *node);
Node* rotateRight(Tree *avl, Node *node);
Tree* load(char *file_name);
int depth(Node* node);
void update_depth(Node *node);
Node* balance(Tree *avl, Node *node);


Node* createNode(char *word)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->word, word);
    new_node->left = new_node->right = NULL;
    new_node->depth = 1;
    return new_node;
}

int max(int a, int b)
{
    return a > b? a : b;
}

void addNode(Tree *avl, Node *new_node)
{
    if (avl->root == NULL)
    {
        avl->root = new_node;
        return;
    }
    Node *parent = NULL, *current = avl->root;
    int compare = 0;
    while (current != NULL)
    {
        compare = strcasecmp(new_node->word, current->word);
        if (compare == 0)
        {
            printf("Word (%s) already exists.\n", new_node->word);
            return;
        }
        parent = current;
        current = (compare<0)? current->left:current->right;
    }
    compare < 0? (parent->left = new_node) : (parent->right = new_node);
    new_node->parent = parent;
    while(parent)
    {
        update_depth(parent);
        parent = balance(avl, parent)->parent;
    }
    return;
}

void search(Tree *avl, char *word)
{
    if(!avl->root)
    {
        printf("Dictionary is empty.\n");
        return;
    }
    Node *current = avl->root, *parent;
    int compare=0;
    while(current)
    {
        compare = strcasecmp(current->word, word);
        if (compare == 0)
        {
            printf("%s: Word Exists.\n", word);
            return;
        }
        parent = current;
        current = (compare < 0)? current->right : current->left;
    }
    printf("%s: Word Not Found. Suggestions: %s, %s, %s\n", word, parent->word, getSuccessor(parent)->word, getPredecessor(parent)->word);
    return;
}

Node* getSuccessor(Node *node)
{
    // Check if Node has Right Child => get the left most child in the right subtree.
    if(node->right)
    {
        Node *current = node->right;
        while(current->left)
        {
            current = current->left;
        }
        return current;
    }
    // Traversing upward until find a larger word.
    Node *current = node;
    while (current->parent)
    {
        current = current->parent;
        if (strcasecmp(node->word, current->word) < 0)
            return current;
    }
    return node;
}

Node* getPredecessor(Node *node)
{
    // Check if Node has Left Child => get the right most child in the left subtree.
    if(node->left)
    {
        Node *current = node->left;
        while(current->right)
        {
            current = current->right;
        }
        return current;
    }
    // Traversing upward until find a smaller word.
    Node *current = node;
    while (current->parent)
    {
        current = current->parent;
        if (strcasecmp(node->word, current->word) > 0)
            return current;
    }
    return node;
}

Node* rotateLeft(Tree *avl, Node *node)
{
    Node* new_node = node->right;
    if(node->parent)
    {
        if (node->parent->right == node) node->parent->right = new_node;
        else node->parent->left = new_node;
    }
    else avl->root = new_node;
    new_node->parent = node->parent;
    node->parent = new_node;
    node->right = new_node->left;
    if (node->right) node->right->parent = node;
    new_node->left = node;

    update_depth(node);
    update_depth(new_node);
    return new_node;
}

Node* rotateRight(Tree *avl, Node *node)
{
    Node* new_node = node->left;
    if(node->parent)
    {
        if (node->parent->left == node) node->parent->left = new_node;
        else node->parent->right = new_node;
    }
    else avl->root = new_node;
    new_node->parent = node->parent;
    node->parent = new_node;
    node->left = new_node->right;
    if (node->left) node->left->parent = node;
    new_node->right = node;

    update_depth(node);
    update_depth(new_node);
    return new_node;
}

Node* balance(Tree *avl, Node *node)
{
    if (depth(node->left) - depth(node->right) > 1)
    {
        if (depth(node->left->left) > depth(node->left->right))
        {
            node = rotateRight(avl, node);
        }
        else
        {
            rotateLeft(avl, node->left);
            node = rotateRight(avl, node);
        }
    }
    else if (depth(node->right) - depth(node->left) > 1)
    {
        if (depth(node->right->right) > depth(node->right->left))
        {
            node = rotateLeft(avl, node);
        }
        else
        {
            rotateRight(avl, node->right);
            node = rotateLeft(avl, node);
        }
    }
    return node;
}

Tree* load(char *file_name)
{
    Tree *avl = (Tree*)malloc(sizeof(Tree));
    FILE *dictionary = fopen(file_name, "r");
    char word[50];
    if (dictionary == NULL)
    {
        printf("Dictionary File does not exist.");
        exit(EXIT_FAILURE);
    }
    while(!feof(dictionary))
    {
        char last[30];
        fscanf(dictionary,"%s\n",last);
        addNode(avl, createNode(last));

    }
    return avl;
}

int depth(Node* node)
{
    return node? node->depth : 0;
}

void update_depth(Node *node)
{
    node->depth = 1 + max(depth(node->left), depth(node->right));
}

void print_tree_indent(Node *node, int indent)
{
    int ix;
    for (ix = 0; ix < indent; ix++) printf(" ");
    if (!node) printf("Empty child\n");
    else
    {
        printf("%s\n", node->word);
        print_tree_indent(node->left, indent + 4);
        print_tree_indent(node->right, indent + 4);
    }
}

void print_tree(Node *node)
{
    print_tree_indent(node, 0);
}

int main()
{
    Tree* avl = load("English_Dictionary.txt");
    //uncomment the following line to print the tree.
    //print_tree(avl->root);
    /*char *word;
    LINE20:
    printf("Enter Your Word : ");
    scanf("%s",word);
    search(avl, word);
    goto LINE20;*/

    printf("Enter sentence : ");
    for(;;)
    {
        char w[30];
        scanf("%s",w);
        search(avl,w);
    }
}

