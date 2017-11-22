#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "extern.h"
#include "parser.h"

extern FILE* fp;

node* parser()
{
	node* root = program();

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);
	
	if (returnStateID(token->tokenID) == "EOFTOK")
	{
		free(token);
	}
	else
	{
		free(root);
		free(token->tokenInstance);
		free(token);
		fprintf(stderr, "No EOFTOK\n");
		exit(-1);
	}

	printf("Success.\n");
	return root;
}

// <program> -> <vars> <block>
node* program()
{

	node* p = malloc(sizeof(node));
	p->func = "<program>";
	p->child1 = vars();
	p->child2 = block();
	return p;
}

// <block> -> Begin <vars> <stats> End
node* block()
{
	node* p = malloc(sizeof(node));
	tokenType* beginTok = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(beginTok->tokenID), beginTok->tokenInstance, beginTok->lineNum);
	p->func = "<block>";
	if (returnStateID(beginTok->tokenID) == "BGNTOK")
	{
		free(beginTok->tokenInstance);
		free(beginTok);
		p->child1 = vars();
		p->child2 = stats();

		tokenType* endToken = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(endToken->tokenID), endToken->tokenInstance, endToken->lineNum);

		if (returnStateID(endToken->tokenID) == "ENDTOK")
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
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: No 'Begin' keyword on line %d.\n", beginTok->lineNum);
		free(p);
		free(beginTok->tokenInstance);
		free(beginTok);
		exit(-1);
	}

	return p;
}

// <vars> -> Var Identifier <mvars> | ε
node* vars()
{
	node* p = malloc(sizeof(node));
	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);
	p->func = "<vars>";
	if (returnStateID(token->tokenID) == "VRTOK")
	{
		free(token->tokenInstance);
		free(token);

		tokenType* idTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(idTok->tokenID), idTok->tokenInstance, idTok->lineNum);

		if (returnStateID(idTok->tokenID) == "IDTOK")
		{
			p->tok = idTok;
			// free(idTok->tokenInstance);
			// free(idTok);
			p->child1 = mvars();
		}
		else
		{
			fprintf(stderr, "Error: No id on line %d\n", idTok->lineNum);
			free(p);
			free(idTok->tokenInstance);
			free(idTok);
			exit(-1);
		}
	}
	else
	{
		free(p);
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);
	p->func = "<mvars>";

	if (returnStateID(token->tokenID) == "PRDTOK")
	{
		free(p);
		free(token->tokenInstance);
		free(token);
	}
	else if (returnStateID(token->tokenID) == "CMMTOK")
	{
		free(token->tokenInstance);
		free(token);
		tokenType* idTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(idTok->tokenID), idTok->tokenInstance, idTok->lineNum);

		if (returnStateID(idTok->tokenID) == "IDTOK")
		{
			p->tok = idTok;
			// free(idTok->tokenInstance);
			// free(idTok);
			p->child1 = mvars();
		}
		else
		{
			fprintf(stderr, "Error: Expected id on line %d.\n", idTok->lineNum);
			free(p);
			free(idTok->tokenInstance);
			free(idTok);
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '.' or ',' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(-1);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "PLSTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child2 = expr();
	}
	else if (returnStateID(token->tokenID) == "MNSTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child2 = expr();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "PRCNTTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child2 = M();
	}
	else if (returnStateID(token->tokenID) == "STRTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child2 = M();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "PRNTHSS_LFTTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->child1 = F();

		tokenType* rightTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(rightTok->tokenID), rightTok->tokenInstance, rightTok->lineNum);

		if (returnStateID(rightTok->tokenID) == "PRNTHSS_RGHTTOK")
		{
			free(rightTok->tokenInstance);
			free(rightTok);
		}
		else
		{
			fprintf(stderr, "Error: Missing ')' on line %d.\n", rightTok->lineNum);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(-1);
		}
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "BRCKT_LFTTOK")
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();

		tokenType* rightTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(rightTok->tokenID), rightTok->tokenInstance, rightTok->lineNum);

		if (returnStateID(rightTok->tokenID) == "BRCKT_RGHTTOK")
		{
			free(rightTok->tokenInstance);
			free(rightTok);
		}
		else
		{
			fprintf(stderr, "Error: Missing ']' on line %d.\n", rightTok->lineNum);
			free(rightTok->tokenInstance);
			free(rightTok);
			exit(-1);
		}
	}
	else if (returnStateID(token->tokenID) == "IDTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "DGGTTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(token);
		exit(-1);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "INPTTOK" || 
		returnStateID(token->tokenID) == "OTPTTOK" || 
		returnStateID(token->tokenID) == "BGNTOK" || 
		returnStateID(token->tokenID) == "CHCKTOK" || 
		returnStateID(token->tokenID) == "LPTOK" || 
		returnStateID(token->tokenID) == "IDTOK")
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);

		p->child1 = stat();
		p->child2 = mStat();
	}
	else
	{
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
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
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "INPTTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = in();
	}
	else if (returnStateID(token->tokenID) == "OTPTTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = out();
	}
	else if (returnStateID(token->tokenID) == "BGNTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		long int SIZE = strlen(token->tokenInstance);
		fseek(fp, -SIZE, SEEK_CUR);
		free(token->tokenInstance);
		free(token);

		p->child1 = block();
	}
	else if (returnStateID(token->tokenID) == "CHCKTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = if_stmt();
	}
	else if (returnStateID(token->tokenID) == "LPTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = loop();
	}
	else if (returnStateID(token->tokenID) == "IDTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = assign();
	}
	else
	{
		fprintf(stderr, "Error: Invalid statement on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(token);
		exit(-1);
	}

	return p;
}

node* in()
{
	node* p = malloc(sizeof(node));
	p->func = "<in>";

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "IDTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;

		tokenType* semiTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(semiTok->tokenID), semiTok->tokenInstance, semiTok->lineNum);

		if (returnStateID(semiTok->tokenID) == "SMCLNTOK")
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
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Expected id after keyword 'In' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(-1);
	}

	return p;
}

node* out()
{
	node* p = malloc(sizeof(node));
	p->func = "<out>";
	p->child1 = expr();

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "SMCLNTOK")
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
		exit(-1);
	}

	return p;
}

node* if_stmt()
{
	node* p = malloc(sizeof(node));
	p->func = "<if_stmt>";

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "BRCKT_LFTTOK")
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();
		p->child2 = RO();
		p->child3 = expr();

		tokenType* rightTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(rightTok->tokenID), rightTok->tokenInstance, rightTok->lineNum);

		if (returnStateID(rightTok->tokenID) == "BRCKT_RGHTTOK")
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
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(-1);
	}

	return p;
}

node* loop()
{
	node* p = malloc(sizeof(node));
	p->func = "<loop>";

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "BRCKT_LFTTOK")
	{
		free(token->tokenInstance);
		free(token);

		p->child1 = expr();
		p->child2 = RO();
		p->child3 = expr();

		tokenType* rightTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(rightTok->tokenID), rightTok->tokenInstance, rightTok->lineNum);

		if (returnStateID(rightTok->tokenID) == "BRCKT_RGHTTOK")
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
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing '[' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(-1);
	}

	return p;
}

node* assign()
{
	node* p = malloc(sizeof(node));
	p->func = "<assign>";

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "CLNTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
		p->child1 = expr();

		tokenType* semiTok = (tokenType*) (intptr_t) scanner();
		printf("%s %s %d\n", returnStateID(semiTok->tokenID), semiTok->tokenInstance, semiTok->lineNum);

		if (returnStateID(semiTok->tokenID) == "SMCLNTOK")
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
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "Error: Missing ':' on line %d.\n", token->lineNum);
		free(p);
		free(token->tokenInstance);
		free(token);
		exit(-1);
	}

	return p;
}

node* RO()
{
	node* p = malloc(sizeof(node));
	p->func = "<RO>";

	tokenType* token = (tokenType*) (intptr_t) scanner();
	printf("%s %s %d\n", returnStateID(token->tokenID), token->tokenInstance, token->lineNum);

	if (returnStateID(token->tokenID) == "LSSRTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "LSSR_EQTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "GRTRTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "GRTR_EQTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "CMPRTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else if (returnStateID(token->tokenID) == "NTEQTOK")
	{
		// free(token->tokenInstance);
		// free(token);
		p->tok = token;
	}
	else
	{
		fprintf(stderr, "Error: No operator on line %d.\n", token->lineNum);
		free(token->tokenInstance);
		free(p);
		free(token);
		exit(-1);
	}

	return p;
}

char* returnStateID(enum token_enum state)
{
	switch(state)
	{
		case EOFTOK:
			return "EOFTOK";
		case IDTOK:
			return "IDTOK";
		case DGGTTOK:
			return "DGGTTOK";
		case EQLTOK:
			return "EQLTOK";
		case LSSRTOK:
			return "LSSRTOK";
		case GRTRTOK:
			return "GRTRTOK";
		case CLNTOK:
			return "CLNTOK";
		case PLSTOK:
			return "PLSTOK";
		case MNSTOK:
			return "MNSTOK";
		case STRTOK:
			return "STRTOK";
		case DVDTOK:
			return "DVDTOK";
		case ANDTOK:
			return "ANDTOK";
		case PRCNTTOK:
			return "PRCNTTOK";
		case PRDTOK:
			return "PRDTOK";
		case PRNTHSS_LFTTOK:
			return "PRNTHSS_LFTTOK";
		case PRNTHSS_RGHTTOK:
			return "PRNTHSS_RGHTTOK";
		case CMMTOK:
			return "CMMTOK";
		case CRLYBRCKTLFTTOK:
			return "CRLYBRCKTLFTTOK";
		case CRLYBRCKTRGHTTOK:
			return "CRLYBRCKTRGHTTOK";
		case SMCLNTOK:
			return "SMCLNTOK";
		case BRCKT_LFTTOK:
			return "BRCKT_LFTTOK";
		case BRCKT_RGHTTOK:
			return "BRCKT_RGHTTOK";
		case CMMNTTOK:
			return "CMMNTTOK";
		case CMPRTOK:
			return "CMPRTOK";
		case LSSR_EQTOK:
			return "LSSR_EQTOK";
		case GRTR_EQTOK:
			return "GRTR_EQTOK";
		case NTEQTOK:
			return "NTEQTOK";
		case ID_UNDRSCR_ERR:
			return "ID_UNDRSCR_ERR: No token starts with '_'.";
		case ID_DGGT_ERR:
			return "ID_DGGT_ERR: ID token cannot start with digit.";
		case BNG_ERR:
			return "BNG_ERR: '!' Can not be without '='.";
		case CMMNT_ERR:
			return "CMMNT_ERR: No closing comment found.";
		case UNKWN_ERR:
			return "UNKWN_ERR: Uknown character.";
		case BGNTOK:
			return "BGNTOK";
		case ENDTOK:
			return "ENDTOK";
		case CHCKTOK:
			return "CHCKTOK";
		case LPTOK:
			return "LPTOK";
		case VDTOK:
			return "VDTOK";
		case VRTOK:
			return "VRTOK";
		case RTRNTOK:
			return "RTRNTOK";
		case INPTTOK:
			return "INPTTOK";
		case OTPTTOK:
			return "OTPTTOK";
		case PRGRMTOK:
			return "PRGRMTOK";
	}
}
