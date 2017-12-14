#include <stdio.h>
#include <stdlib.h>
#include "codeGenerator.h"
#include "extern.h"
#include "stack.h"
#include "staticSemantics.h"
#include "string.h"
#include "token.h"

extern FILE* outputFile;
static StackNode* stack_node = NULL;
static int localVarCount = 0, globalVarCount = 0;
static int gvFlag = 1;
static int funcs = 0;

void startStaticSemantics(node* p)
{
	traverseTree(p, 0);

	// Pop the last local vars from stack
	int i;
	for (i = 0; i < localVarCount; i++)
	{
		free(pop(&stack_node));
		POP();
	}

	// Stop asm program
	STOP();

	// Pop global vars, write them to asm file
	for (i = 0; i < globalVarCount; i++)
	{
		char* gv = pop(&stack_node);
		GLOBVARS(gv);
		free(gv);
	}

	// Add temporaries
	GLOBVARS("T#");
	GLOBVARS("V#");
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

	if (strcmp(p->func, "<block>") == 0)
	{
		gvFlag = 0;

		if (p->child1)
			traverseTree(p->child1, ++level);
		globalVarCount += localVarCount;
		if (p->child2)
			traverseTree(p->child2, ++level);
		if (p->child3)
			traverseTree(p->child3, ++level);
		if (p->child4)
			traverseTree(p->child4, ++level);

		int i;
		for(i = 0; i < localVarCount; i++)
		{
			free(pop(&stack_node));
			POP();
		}
		globalVarCount -= localVarCount;
		localVarCount = 0;
	}
	else if (strcmp(p->func, "<vars>") == 0 || strcmp(p->func, "<mvars>") == 0)
	{
		if (p->tok)
		{
			if (gvFlag)
			{
				if (find(stack_node, p->tok->tokenInstance, globalVarCount++) > -1)
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}
				push(&stack_node, p->tok);
			}
			else
			{
				// Check local stack
				if (find(stack_node, p->tok->tokenInstance, localVarCount++ - 1) > -1)
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}
				push(&stack_node, p->tok);
				PUSH();
				STACKW(0);
			}
		}

		if (p->child1)
			traverseTree(p->child1, ++level);
	}
	else if (strcmp(p->func, "<expr>") == 0)
	{
		if (p->child1) {
			traverseTree(p->child1, ++level);
			PUSH();
			STACKW(0);
		}
		if (p->child2) {
			traverseTree(p->child2, ++level);
		}

		STACKR(0);
		POP();
		if (p->tok)
		{
			if (strcmp(returnStateID(p->tok->tokenID), "PLSTOK") == 0)
			{
				ADD("T#");
			}
			if (strcmp(returnStateID(p->tok->tokenID), "MNSTOK") == 0)
			{
				SUB("T#");
			}
		}
		STORE("T#\n"); // Store ACC in temp		
	}
	else if (strcmp(p->func, "<M>") == 0)
	{
		if (p->child1)
		{
			traverseTree(p->child1, ++level);
			PUSH();
			STACKW(0);
		}
		if (p->child2)
			traverseTree(p->child2, ++level);

		STACKR(0);
		POP();
		if (p->tok)
		{
			if (strcmp(returnStateID(p->tok->tokenID), "PRCNTTOK") == 0)
			{
				DIV("T#");
			}
			if (strcmp(returnStateID(p->tok->tokenID), "STRTOK") == 0)
			{
				MULT("T#");
			}
		}
		STORE("T#"); // Store ACC in temp			
	}
	else if (strcmp(p->func, "<F>") == 0)
	{
		if (p->child1)
		{
			traverseTree(p->child1, ++level);
			if (strcmp(p->child1->func, "<F>") == 0)
				MULT("-1");
		}
		if (p->child2)
			traverseTree(p->child2, ++level);
	}
	else if (strcmp(p->func, "<R>") == 0)
	{
		if (p->tok)
		{
			if (strcmp(returnStateID(p->tok->tokenID), "IDTOK") == 0)
				if (find(stack_node, p->tok->tokenInstance, globalVarCount + localVarCount) < 0)
				{
					fprintf(stderr, "Error. Var %s not declared\n", p->tok->tokenInstance);
					exit(1);
				}
			LOAD(p->tok->tokenInstance);
		}
		if (p->child1)
			traverseTree(p->child1, ++level);
	}
	else if (strcmp(p->func, "<in>") == 0)
	{
		if (strcmp(returnStateID(p->tok->tokenID), "IDTOK") == 0)
		{
			if (find(stack_node, p->tok->tokenInstance, globalVarCount + localVarCount) < 0)
			{
					fprintf(stderr, "Error. Var %s not declared\n", p->tok->tokenInstance);
					exit(1);
			}
			READ(p->tok->tokenInstance);
		}
	}
	else if (strcmp(p->func, "<out>") == 0)
	{
		if (p->child1)
		{
			traverseTree(p->child1, ++level);	
			WRITE("T#\n"); // Output temp
		}
	}
	else if (strcmp(p->func, "<if_stmt>") == 0)
	{
		char* funcName = returnStateID(p->child2->tok->tokenID);
		size_t SIZE = strlen(funcName) + 5 + 1; // +5 = 'AFTER', +1 = '\0'
		char afterFuncName[SIZE];
		strcpy(afterFuncName, "AFTER"); strcat(afterFuncName, funcName);

		if (p->child1)
			traverseTree(p->child1, ++level);
		if (p->child2)
		{
			COPY("V#", "T#");
			traverseTree(p->child2, ++level);
		}
		if (p->child3)
		{
			traverseTree(p->child3, ++level);
			LOAD("V#");
			SUB("T#");
			if (p->child2->tok)
			{
				if (strcmp(funcName, "LSSRTOK") == 0)
					BRNEG(funcName, funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "LSSREQTOK") == 0)
					BRZNEG(funcName, funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "GRTRTOK") == 0)
					BRPOS(returnStateID(p->child2->tok->tokenID), funcs);
				else if (strcmp(funcName, "GRTREQTOK") == 0)
					BRZPOS(funcName, funcs);
				else if (strcmp(funcName, "CMPRTOK") == 0)
					BRZERO(funcName, funcs);

				BR(afterFuncName, funcs);
			}
		}
		if (p->child4)
		{
			FUNC(funcName, funcs++);
			traverseTree(p->child4, ++level);
			FUNC(afterFuncName, --funcs);
			NOOP();
		}

	}
	else if (strcmp(p->func, "<loop>") == 0)
	{
		char* funcName = returnStateID(p->child2->tok->tokenID);
		size_t SIZE = strlen(funcName) + 5 + 1; // +5 = 'AFTER', +1 = '\0'
		char afterFuncName[SIZE];
		strcpy(afterFuncName, "AFTER"); strcat(afterFuncName, funcName);

		if (p->child1)
			traverseTree(p->child1, ++level);
		if (p->child2)
		{
			COPY("V#", "T#");
			traverseTree(p->child2, ++level);
		}
		if (p->child3)
		{
			traverseTree(p->child3, ++level);
			LOAD("V#");
			SUB("T#");
			if (p->child2->tok)
			{
				if (strcmp(funcName, "LSSRTOK") == 0)
					BRNEG(funcName, funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "LSSREQTOK") == 0)
					BRZNEG(funcName, funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "GRTRTOK") == 0)
					BRPOS(returnStateID(p->child2->tok->tokenID), funcs);
				else if (strcmp(funcName, "GRTREQTOK") == 0)
					BRZPOS(funcName, funcs);
				else if (strcmp(funcName, "CMPRTOK") == 0)
					BRZERO(funcName, funcs);

				BR(afterFuncName, funcs);
			}
		}
		if (p->child4)
		{
			FUNC(funcName, funcs++);
			traverseTree(p->child4, ++level);
			if (p->child2->tok)
			{
				if (strcmp(funcName, "LSSRTOK") == 0)
					BRNEG(funcName, --funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "LSSREQTOK") == 0)
					BRZNEG(funcName, --funcs);
				else if (strcmp(returnStateID(p->child2->tok->tokenID), "GRTRTOK") == 0)
					BRPOS(returnStateID(p->child2->tok->tokenID), --funcs);
				else if (strcmp(funcName, "GRTREQTOK") == 0)
					BRZPOS(funcName, --funcs);
				else if (strcmp(funcName, "CMPRTOK") == 0)
					BRZERO(funcName, --funcs);
			}
			FUNC(afterFuncName, funcs);
			NOOP();
		}
	}
	else if (strcmp(p->func, "<assign>") == 0)
	{
		if (strcmp(returnStateID(p->tok->tokenID), "IDTOK") == 0)
			if (find(stack_node, p->tok->tokenInstance, globalVarCount + localVarCount) < 0)
			{
					fprintf(stderr, "Error. Var %s not declared\n", p->tok->tokenInstance);
					exit(1);
			}
		if (p->child1)
		{
			traverseTree(p->child1, ++level);
			STORE("T#");
			COPY(p->tok->tokenInstance, "T#");
		}
	}
	else
	{
		if (p->child1)
			traverseTree(p->child1, ++level);
		if (p->child2)
			traverseTree(p->child2, ++level);
		if (p->child3)
			traverseTree(p->child3, ++level);
		if (p->child4)
			traverseTree(p->child4, ++level);
	}	
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
