#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

void pushToStack(node* p);
void startStaticSemantics(node* p, FILE* fp);
void traverseTree(node* p, int level);

#endif
