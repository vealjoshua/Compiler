#ifndef TREE_H
#define TREE_H

#include "token.h"

struct bin_tree 
{
	char* func;
    tokenType* tok;
    struct bin_tree* child1;
    struct bin_tree* child2;
    struct bin_tree* child3;
    struct bin_tree* child4;
}typedef node;

#endif
