#include <stdio.h>
#include <stdlib.h>
#include "testTree.h"
#include "token.h"

void testTree(node* p, int level)
{
	if (p->tok)
		printf("%*c%s %s\n", level, ' ', p->func, p->tok->tokenInstance);
	else
		printf("%*c%s\n", level, ' ', p->func);
	if (p->child1)
		testTree(p->child1, ++level);
	if (p->child2)
		testTree(p->child2, ++level);
	if (p->child3)
		testTree(p->child3, ++level);
	if (p->child4)
		testTree(p->child4, ++level);
}

void deleteTree(node* p)
{
	if (p->child1)
		deleteTree(p->child1);
	if (p->child2)
		deleteTree(p->child2);
	if (p->child3)
		deleteTree(p->child3);
	if (p->child4)
		deleteTree(p->child4);
	if (p->tok)
	{
		free(p->tok->tokenInstance);
		free(p->tok);
	}
	free(p);
}
