#include <stdio.h>

typedef struct
Node {
    int data;
    node_t * left;
    node_t * right;

} node_t;

int main(int argc, char ** argv);
node_t * createNode(int value);
node_t * insertLeft(node_t * root, int value);

node_t * createNode(int value){
    node_t * newNode = malloc(sizeof(node_t));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
}

node_t * insertLeft(node_t * root, int value){

    root->left = createNode(value);
    return root->left;

}

node_t * insertLeft(node_t * root, int value){

    root->right = createNode(value);
    return root->right;

}

int main(int agrc, char ** argv){



    return 0;
}