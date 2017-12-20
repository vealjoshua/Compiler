#include "tree.h"

typedef struct node
{
	char* func = "\0";
    tokenType* tok = NULL;
    struct node* child1 = NULL;
    struct node* child2 = NULL;
    struct node* child3 = NULL;
    struct node* child4 = NULL;
} node;