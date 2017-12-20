#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

void pushToStack(node* p);
void startStaticSemantics(node* p);
void traverseTree(node* p, int level, int* vc);

#endif
