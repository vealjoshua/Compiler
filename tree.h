#ifndef TREE_H
#define TREE_H

#include "token.h"

typedef struct node
{
	char* func;
    tokenType* tok;
    struct node* child1;
    struct node* child2;
    struct node* child3;
    struct node* child4;
} node;

#endif
