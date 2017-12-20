#include <stdio.h>
#include <stdlib.h>
#include "codeGenerator.h"
#include "extern.h"
#include "stack.h"
#include "staticSemantics.h"
#include "string.h"
#include "token.h"

extern FILE* outputFile;
static StackNode* gv_stack = NULL; 
static StackNode* lv_stack = NULL;
static int localVarCount = 0, globalVarCount = 0;
static int gvFlag = 1;
static int funcs = 0;

void startStaticSemantics(node* p)
{
	int vc = 0;
	traverseTree(p, 0, &vc);

	// Pop the last local vars from stack
	int i;
	// for (i = 0; i < localVarCount; i++)
	// {
	// 	free(pop(&stack_node));
	// 	POP();
	// }

	// Stop asm program
	STOP();

	// Pop global vars, write them to asm file
	for (i = 0; i < globalVarCount; i++)
	{
		char* gv = pop(&gv_stack);
		GLOBVARS(gv);
		free(gv);
	}

	// Add temporaries
	GLOBVARS("T#");
	GLOBVARS("V#");
}

void traverseTree(node* p, int level, int* vc)
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
		int vc = 0;
		if (p->child1)
			traverseTree(p->child1, ++level, &vc);
		localVarCount += vc;
		if (p->child2)
			traverseTree(p->child2, ++level, &vc);
		int i;
		for (i = 0; i < vc; i++)
		{
			pop(&lv_stack);
			POP();
		}
	}
	else if (strcmp(p->func, "<vars>") == 0 || strcmp(p->func, "<mvars>") == 0)
	{
		if (p->tok)
		{
			if (gvFlag)
			{
				if(find(gv_stack, p->tok->tokenInstance, globalVarCount++) < 0)
				{
					push(&gv_stack, p->tok);
				}
				else
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}	
			}
			else
			{
				if(find(lv_stack, p->tok->tokenInstance, (*vc)++) < 0)
				{
					push(&lv_stack, p->tok);
				}
				else
				{
					fprintf(stderr, "Error. Var %s previously declared\n", p->tok->tokenInstance);
					exit(1);
				}	
			}
		}
		if (p->child1)
			traverseTree(p->child1, ++level, vc);
	}
	else if (strcmp(p->func, "<expr>") == 0)
	{
		if (p->child1) {
			traverseTree(p->child1, ++level, vc);
			PUSH();
			STACKW(0);
		}
		if (p->child2) {
			traverseTree(p->child2, ++level, vc);
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
			traverseTree(p->child1, ++level, vc);
			PUSH();
			STACKW(0);
		}
		if (p->child2)
			traverseTree(p->child2, ++level, vc);

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
			traverseTree(p->child1, ++level, vc);
			if (strcmp(p->child1->func, "<F>") == 0)
				MULT("-1");
		}
		if (p->child2)
			traverseTree(p->child2, ++level, vc);;
	}
	else if (strcmp(p->func, "<R>") == 0)
	{
		if (p->tok)
		{
			if (strcmp(returnStateID(p->tok->tokenID), "IDTOK") == 0)
			{
				if (find(lv_stack, p->tok->tokenInstance, localVarCount) > -1)
				{
					STACKR(0);
					// LOAD("T#");
				}
				else if (find(gv_stack, p->tok->tokenInstance, globalVarCount) > -1)
				{
					LOAD(p->tok->tokenInstance);
				}
				else
				{
					fprintf(stderr, "Error. Var %s not declared\n", p->tok->tokenInstance);
					exit(1);
				}
			}
			else
				LOAD(p->tok->tokenInstance);
		}
		if (p->child1)
			traverseTree(p->child1, ++level, vc);
	}
	else if (strcmp(p->func, "<in>") == 0)
	{
		if (p->tok)
		{
			if (strcmp(returnStateID(p->tok->tokenID), "IDTOK") == 0)
			{
				if(find(lv_stack, p->tok->tokenInstance, localVarCount) > -1)
				{
					READ("T#");
					LOAD("T#");
					PUSH();
					STACKW(0);
				}
				else if(find(gv_stack, p->tok->tokenInstance, globalVarCount) > -1)
				{
					READ(p->tok->tokenInstance);
				}
			}
		}
	}
	else if (strcmp(p->func, "<out>") == 0)
	{
		if (p->child1)
		{
			traverseTree(p->child1, ++level, vc);	
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
			traverseTree(p->child1, ++level, vc);
		if (p->child2)
		{
			COPY("V#", "T#");
			traverseTree(p->child2, ++level, vc);;
		}
		if (p->child3)
		{
			traverseTree(p->child3, ++level, vc);
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
			traverseTree(p->child4, ++level, vc);
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
			traverseTree(p->child1, ++level, vc);
		if (p->child2)
		{
			COPY("V#", "T#");
			traverseTree(p->child2, ++level, vc);;
		}
		if (p->child3)
		{
			traverseTree(p->child3, ++level, vc);
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
			traverseTree(p->child4, ++level, vc);
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
			if (find(gv_stack, p->tok->tokenInstance, globalVarCount) < 0 && find(lv_stack, p->tok->tokenInstance, localVarCount) < 0)
			{
					fprintf(stderr, "Error. Var %s not declared\n", p->tok->tokenInstance);
					exit(1);
			}
		if (p->child1)
		{
			traverseTree(p->child1, ++level, vc);
			STORE("T#");
			COPY(p->tok->tokenInstance, "T#");
		}
	}
	else
	{
		if (p->child1)
			traverseTree(p->child1, ++level, vc);
		if (p->child2)
			traverseTree(p->child2, ++level, vc);;
		if (p->child3)
			traverseTree(p->child3, ++level, vc);
		if (p->child4)
			traverseTree(p->child4, ++level, vc);
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
