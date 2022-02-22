#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

typedef struct Node{
    char word[50];
    struct Node *left, *right, *parent;
    int depth;
}Node;

typedef struct Tree{
    Node *root;
}Tree;

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

#endif // MAIN_H_INCLUDED
