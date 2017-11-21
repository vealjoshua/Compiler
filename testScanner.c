#include <stdlib.h>
#include <string.h>
#include "testScanner.h"
#include "token.h"

void testScanner(FILE* fp)
{
	size_t SIZE = 256;
	char str[SIZE];
	int lineNumber = 1;

	while(fgets(str, SIZE, fp))
	{
		// Remove the '\n' placed by fgets
		// size_t length = strlen(str);
		// if (length > 0 && str[length-1] == '\n')
		// 	str[--length] = '\0';
		if (*str != '\n')
		{
			int numOfTokens = 0;

			tokenType** tokens = scanner(str, lineNumber++, &numOfTokens);

			int i = 0;

			for (i; i < numOfTokens; i++)
			{
				char* stateID = returnStateID(tokens[i]->tokenID);
				if (tokens[i]->tokenID < 0)
				{
					printf("Error on line %d: %s\n", tokens[i]->lineNum, stateID);
					exit(-1);
				}
				printf("%s %s %d\n", stateID, tokens[i]->tokenInstance, tokens[i]->lineNum);
				free(tokens[i]->tokenInstance);
				free(tokens[i]);
				
			}
			free(tokens);
		}
	}

	printf("EOFTOK\n");
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
		case CMMNTOK:
			return "CMMNTOK";
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
		case CMMT_ERR:
			return "CMMT_ERR: No closing comment found.";
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

/*
	ID_UNDRSCR_ERR 	= -1,
	ID_DGGT_ERR 	= -2,
	BNG_ERR		= -3,
	CMMT_ERR	= -4,
	UNKWN_ERR	= -1000,
	
	EOFTOK		= 1000,
	IDTOK		= 1001,
	DGGTTOK		= 1002,
	EQLTOK		= 1003,
	LSSRTOK		= 1004,
	GRTRTOK		= 1005,
	CLNTOK		= 1006,
	PLSTOK		= 1007,
	MNSTOK		= 1008,
	STRTOK		= 1009,
	DVDTOK		= 1010,
	ANDTOK		= 1011,
	PRCNTTOK	= 1012,
	PRDTOK		= 1013,
	PRNTHSS_LFTTOK	= 1014,
	PRNTHSS_RGHTTOK	= 1015,
	CMMNTOK		= 1016,
	CRLYBRCKTLFTTOK		= 1017,
	CRLYBRCKTRGHTTOK	= 1018,
	SMCLNTOK	= 1019,
	BRCKT_LFTTOK	= 1020,
	BRCKT_RGHTTOK	= 1021,
	CMMNTTOK	= 1022,
	CMPRTOK		= 1023,
	LSSR_EQTOK	= 1024,
	GRTR_EQTOK	= 1025,
	NTEQTOK		= 1026,

	BGNTOK		= 2000,
	ENDTOK		= 2001,
	CHCKTOK		= 2002,
	LPTOK		= 2003,
	VDTOK		= 2004,
	VRTOK		= 2005,
	RTRNTOK		= 2006,
	INPTTOK		= 2007,
	OTPTTOK		= 2008,
	PRGRMTOK	= 2009
*/
