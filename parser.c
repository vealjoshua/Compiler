#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extern.h"
#include "parser.h"
#include "token.h"

extern FILE* inputFile;

node* parser()
{
	node* root = program();

	tokenType* token = (tokenType*) (intptr_t) scanner();
	
	if (strcmp(returnStateID(token->tokenID), "EOFTOK") == 0)
		free(token);
	else
	{
		free(root);
		free(token->tokenInstance);
		free(token);
		fprintf(stderr, "No EOFTOK\n");
		exit(1);
	}

	return root;
}

// <program> -> <vars> <block>
node* program()
{
	node* p = malloc(sizeof(node));
	p->func = "<program>";
	p->child1 = vars();
	p->child2 = block();
	p->child3 = NULL;
	p->child4 = NULL;
	return p;
}

// <block> -> Begin <vars> <stats> End
node* block()
{
	node* p = malloc(sizeof(node));
	p->func = "<block>";

	tokenType* beginTok = (tokenType*) (intptr_t) scanner();
	if (strcmp(returnStateID(beginTok->tokenID), "BGNTOK") == 0)
	{
		free(beginTok->tokenInstance);
		free(beginTok);
		p->child1 = vars();
		p->child2 = stats();
		p->child3 = NULL;
		p->child4 = NULL;

		tokenType* endToken = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(endToken->tokenID), "ENDTOK") == 0)
		{
			free(endToken->tokenInstance);
			free(endToken);
		}
		else
		{
			fprintf(stderr, "Error. No end token on line %d\n", endToken->lineNum);
			free(p);
			free(endToken->tokenInstance);
			free(endToken);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: No 'Begin' keyword on line %d.\n", beginTok->lineNum);
		free(p);
		free(beginTok->tokenInstance);
		free(beginTok);
		exit(1);
	}

	return p;
}

// <vars> -> Var Identifier <mvars> | ε
node* vars()
{
	node* p = malloc(sizeof(node));
	p->func = "<vars>";
	p->child1 = NULL;
	p->child2 = NULL;
	p->child3 = NULL;
	p->child4 = NULL;

	tokenType* token = (tokenType*) (intptr_t) scanner();
	if (strcmp(returnStateID(token->tokenID), "VRTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);

		tokenType* idTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(idTok->tokenID), "IDTOK") == 0)
		{
			p->tok = idTok;
			p->child1 = mvars();
		}
		else
		{
			fprintf(stderr, "Error: No id on line %d\n", idTok->lineNum);
			free(p);
			free(idTok->tokenInstance);
			free(idTok);
			exit(1);
		}
	}
	else
	{
		free(p);
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);
	}

	return p;
}

// <mvars> -> , Identifier <mvars> | .
node* mvars()
{
	node* p = malloc(sizeof(node));
	tokenType* token = (tokenType*) (intptr_t) scanner();
	p->func = "<mvars>";
	p->child1 = NULL;
	p->child2 = NULL;
	p->child3 = NULL;
	p->child4 = NULL;

	if (strcmp(returnStateID(token->tokenID), "PRDTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);
	}
	else if (strcmp(returnStateID(token->tokenID), "CMMTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);
		tokenType* idTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(idTok->tokenID), "IDTOK") == 0)
		{
			p->tok = idTok;
			p->child1 = mvars();
		}
		else
		{
			fprintf(stderr, "Error: Expected id on line %d.\n", idTok->lineNum);
			free(p);
			free(idTok->tokenInstance);
			free(idTok);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '.' or ',' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

// <expr> -> <M> + <expr> | <M> - <expr> | <M>
node* expr()
{
	node* p = malloc(sizeof(node));
	p->func = "<expr>";
	p->child1 = M();
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "PLSTOK") == 0)
	{
		p->tok = token;
		p->child2 = expr();
	}
	else if (strcmp(returnStateID(token->tokenID), "MNSTOK") == 0)
	{
		p->tok = token;
		p->child2 = expr();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);
	}

	return p;
}

node* M()
{
	node* p = malloc(sizeof(node));
	p->func = "<M>";
	p->child1 = F();
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "PRCNTTOK") == 0)
	{
		p->tok = token;
		p->child2 = M();
	}
	else if (strcmp(returnStateID(token->tokenID), "STRTOK") == 0)
	{
		p->tok = token;
		p->child2 = M();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);
	}

	return p;
}

node* F()
{
	node* p = malloc(sizeof(node));
	p->func = "<F>";
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "PRNTHSS_LFTTOK") == 0)
	{
		p->child1 = F();
		tokenType* rightTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(rightTok->tokenID), "PRNTHSS_RGHTTOK") == 0)
		{
			free(rightTok->tokenInstance);
			free(rightTok);
		}
		else
		{
			fprintf(stderr, "Error: Missing ')' on line %d.\n", rightTok->lineNum);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(1);
		}
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);

		p->child1 = R();
	}

	return p;
}

node* R()
{
	node* p = malloc(sizeof(node));
	p->func = "<R>";
	tokenType* token = (tokenType*) (intptr_t) scanner();
	char* stateID = returnStateID(token->tokenID);

	if (strcmp(stateID, "BRCKT_LFTTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();
		tokenType* rightTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(rightTok->tokenID), "BRCKT_RGHTTOK") == 0)
		{
			free(rightTok->tokenInstance);
			free(rightTok);
		}
		else
		{
			fprintf(stderr, "Error: Missing ']' on line %d.\n", rightTok->lineNum);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(1);
		}
	}
	else if (strcmp(stateID, "IDTOK") == 0 || strcmp(stateID, "DGGTTOK") == 0)
		p->tok = token;
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* stats()
{
	node* p = malloc(sizeof(node));
	p->func = "<stats>";
	p->child1 = stat();
	p->child2 = mStat();

	return p;
}

node* mStat()
{
	node* p = malloc(sizeof(node));
	p->func = "<mStat>";
	tokenType* token = (tokenType*) (intptr_t) scanner();
	char* stateID = returnStateID(token->tokenID);

	if (strcmp(stateID, "INPTTOK") == 0 || 
		strcmp(stateID, "OTPTTOK") == 0 || 
		strcmp(stateID, "BGNTOK") == 0 || 
		strcmp(stateID, "CHCKTOK") == 0 || 
		strcmp(stateID, "LPTOK") == 0 || 
		strcmp(stateID, "IDTOK") == 0)
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);

		p->child1 = stat();
		p->child2 = mStat();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);
	}

	return p;
}

node* stat()
{
	node* p = malloc(sizeof(node));
	p->func = "<stat>";
	tokenType* token = (tokenType*) (intptr_t) scanner();
	char* stateID = returnStateID(token->tokenID);

	if (strcmp(stateID, "INPTTOK") == 0)
	{
		p->tok = token;
		p->child1 = in();
	}
	else if (strcmp(stateID, "OTPTTOK") == 0)
	{
		p->tok = token;
		p->child1 = out();
	}
	else if (strcmp(stateID, "BGNTOK") == 0)
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(inputFile, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);

		p->child1 = block();
	}
	else if (strcmp(stateID, "CHCKTOK") == 0)
	{
		p->tok = token;
		p->child1 = if_stmt();
	}
	else if (strcmp(stateID, "LPTOK") == 0)
	{
		p->tok = token;
		p->child1 = loop();
	}
	else if (strcmp(stateID, "IDTOK") == 0)
	{
		p->tok = token;
		p->child1 = assign();
	}
	else
	{
		fprintf(stderr, "Error: Invalid statement on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* in()
{
	node* p = malloc(sizeof(node));
	p->func = "<in>";
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "IDTOK") == 0)
	{
		p->tok = token;
		tokenType* semiTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(semiTok->tokenID), "SMCLNTOK") == 0)
		{
			free(semiTok->tokenInstance);
			free(semiTok);
		}
		else
		{
			fprintf(stderr, "Error: Expected semicolon on line %d.\n", semiTok->lineNum);
			free(token->tokenInstance);
			free(token);
			free(p);
			free(semiTok->tokenInstance);
			free(semiTok);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Expected id after keyword 'In' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* out()
{
	node* p = malloc(sizeof(node));
	p->func = "<out>";
	p->child1 = expr();
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "SMCLNTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);
	}
	else
	{
		fprintf(stderr, "Error: Expected semicolon on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* if_stmt()
{
	node* p = malloc(sizeof(node));
	p->func = "<if_stmt>";
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "BRCKT_LFTTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();
		p->child2 = RO();
		p->child3 = expr();

		tokenType* rightTok = (tokenType*) (intptr_t) scanner();
		if (strcmp(returnStateID(rightTok->tokenID), "BRCKT_RGHTTOK") == 0)
		{
			free(rightTok->tokenInstance);
			free(rightTok);
			p->child4 = stat();
		}
		else
		{
			fprintf(stderr, "Error: Missing ']' on line %d.\n", rightTok->lineNum);
			free(p);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* loop()
{
	node* p = malloc(sizeof(node));
	p->func = "<loop>";
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "BRCKT_LFTTOK") == 0)
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();
		p->child2 = RO();
		p->child3 = expr();
		tokenType* rightTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(rightTok->tokenID), "BRCKT_RGHTTOK") == 0)
		{
			free(rightTok->tokenInstance);
			free(rightTok);
			p->child4 = stat();
		}
		else
		{
			fprintf(stderr, "Error: Missing ']' on line %d.\n", rightTok->lineNum);
			free(p);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* assign()
{
	node* p = malloc(sizeof(node));
	p->func = "<assign>";
	tokenType* token = (tokenType*) (intptr_t) scanner();

	if (strcmp(returnStateID(token->tokenID), "CLNTOK") == 0)
	{

		p->tok = token;
		p->child1 = expr();
		tokenType* semiTok = (tokenType*) (intptr_t) scanner();

		if (strcmp(returnStateID(semiTok->tokenID), "SMCLNTOK") == 0)
		{
			free(semiTok->tokenInstance);
			free(semiTok);
		}
		else
		{
			fprintf(stderr, "Error: Expected semicolon on line %d.\n", semiTok->lineNum);
			free(p);
			free(semiTok->tokenInstance);
			free(semiTok);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing ':' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(1);
	}

	return p;
}

node* RO()
{
	node* p = malloc(sizeof(node));
	p->func = "<RO>";
	tokenType* token = (tokenType*) (intptr_t) scanner();
	char* stateID = returnStateID(token->tokenID);
	if (strcmp(stateID, "LSSRTOK") == 0 ||
		strcmp(stateID, "LSSR_EQTOK") == 0 ||
		strcmp(stateID, "GRTRTOK") == 0 ||
		strcmp(stateID, "GRTR_EQTOK") == 0 ||
		strcmp(stateID, "CMPRTOK") == 0 ||
		strcmp(stateID, "NTEQTOK") == 0)
			p->tok = token;
	else
	{
		fprintf(stderr, "Error: No operator on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(p);
		free(token);
		exit(1);
	}

	return p;
}
