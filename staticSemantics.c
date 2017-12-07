#include <stdio.h>
#include <stdlib.h>
#include "codeGenerator.h"
#include "stack.h"
#include "staticSemantics.h"
#include "token.h"

static StackNode* stack_node = NULL;
static int varCount = 0, gvCount = 0;
static int gvFlag = 1;
FILE* fp;

void startStaticSemantics(node* p, FILE* fp)
{
	traverseTree(p, 0);
	int i;
	for (i = 0; i < varCount + gvCount; i++)
	{
		free(pop(&stack_node));
	}
}

void pushToStack(node* p)
{
	if (strcmp(p->func, "<program>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<block>") == 0)
	{
		int i;
		for(i = 0; i < varCount; i++)
			pop(&stack_node);
		varCount = 0;
		gvFlag = 0;
	}
	else if ((strcmp(p->func, "<vars>") == 0 || strcmp(p->func, "<mvars>") == 0))
	{
		if (p->tok)
		{
			if (gvFlag)
			{
				if (find(stack_node, p->tok->tokenInstance, gvCount++) > -1)
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}
				push(&stack_node, p->tok);			
			}
			else
			{
				// Check local stack
				if (find(stack_node, p->tok->tokenInstance, varCount++) > -1)
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}
				push(&stack_node, p->tok);
			}
		}	
	}
	else if (strcmp(p->func, "<expr>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<M>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<F>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<R>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<stats>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<mStat>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<stat>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<in>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<out>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<if>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<loop>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<assign>") == 0)
	{
		
	}
	else if (strcmp(p->func, "<RO>") == 0)
	{
		
	}
	else
	{
		fprintf(stderr, "Error. Node has a function not in the CFG\n");
		exit(1);
	}
}

void traverseTree(node* p, int level)
{
	if (p->func && p->tok)
		printf("%*c%s %s\n", level, ' ', p->func, p->tok->tokenInstance);
	else if (p->func)
		printf("%*c%s\n", level, ' ', p->func);
	else
	{
		fprintf(stderr, "Error. Node has no function or token\n");
		exit(1);
	}

	pushToStack(p);

	if (p->child1)
		traverseTree(p->child1, ++level);
	if (p->child2)
		traverseTree(p->child2, ++level);
	if (p->child3)
		traverseTree(p->child3, ++level);
	if (p->child4)
		traverseTree(p->child4, ++level);
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
