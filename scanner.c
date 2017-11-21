#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "scanner.h"
#include "token.h"

extern FILE* fp;

tokenType* scanner()
{
	int FATAble[28][30] = 
	{				/*0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17		18		19		20		21		22		23		24		25		26		27		28		29*/
					/*' '	L		D		_		EOF		=		<		>		!		:		+		-		*		/		&		%		.		(		)		,		{		}		;		[		]		#		\n		\0		\t		other*/
		/*0 - WS*/	{0,		1,		2,		-1,		1000,	3,		4,		5,		6,		7,		8,		9,		10,		11,		12,		13,		14,		15,		16,		17,		18,		19,		20,		21,		22,		23,		0,		0,		0,		-1000},
		/*1 - ID*/	{1001,	1,		1,		1,		1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001,	1001, 	1001,	-1000},
		/*2 - DIG*/	{1002,	-2,		2,		-2,		1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	1002,	-1000},
		/*3 - =*/	{1003,	1003,	1003,	1003,	1003,	24,		1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	1003,	-1000},
		/*4 - <*/	{1004,	1004,	1004,	1004,	1004,	25,		1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	1004,	-1000},
		/*5 - >*/	{1005,	1005,	1005,	1005,	1005,	26,		1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	1005,	-1000},
		/*6 - !*/	{-3,	-3,		-3,		-3,		-3,		27,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-3,		-1000},
		/*7 - :*/	{1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	1006,	-1000},
		/*8 - +*/	{1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	1007,	-1000},
		/*9 - -*/	{1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	1008,	-1000},
		/*10 - **/	{1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	1009,	-1000},
		/*11 - /*/	{1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	1010,	-1000},
		/*12 - &*/	{1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	1011,	-1000},
		/*13 - %*/	{1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	1012,	-1000},
		/*14 - .*/	{1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	1013,	-1000},
		/*15 - (*/	{1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	1014,	-1000},
		/*16 - )*/	{1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	1015,	-1000},
		/*17 - ,*/	{1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	1016,	-1000},
		/*18 - {*/	{1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	1017,	-1000},
		/*19 - }*/	{1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	1018,	-1000},
		/*20 - ;*/	{1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	1019,	-1000},
		/*21 - [*/	{1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	1020,	-1000},
		/*22 - ]*/	{1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	1021,	2021,	-1000},
		/*23 - #*/	{23,	23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		23,		1022,	-4,		-4,		23,		-4},
		/*24 - ==*/	{1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	1023,	-1000},
		/*25 - <=*/	{1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	1024,	-1000},
		/*26 - >=*/	{1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	1025,	-1000},
		/*27 - !=*/	{1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	1026,	-1000}
	};

	int state = 0;
	int nextState = 0;
	int tokLen = 0;
	static int lineNumber = 1;
	char* strHEAD = malloc(sizeof(char));
	char* str = strHEAD;

	int c = 0, nextC = 0;
	while((c = fgetc(fp)) != EOF) {
		nextC = fgetc(fp);
		if (nextC == EOF)
			fseek(fp, 0, SEEK_END);
		else
			fseek(fp, -1L, SEEK_CUR);

		int charNum = determineCharNum(c);
		int nextCharNum = determineCharNum(nextC);
		state = FATAble[state][charNum];
		if (state != 23 && nextState == 1022)
			state = 0;
		nextState = FATAble[state][nextCharNum];

		// testing purposes
		// printf("state = %d, char = '%c', charNum = %d\n", state, c, charNum);
		// printf("nextState = %d, nextChar = '%c', nextCharNum = %d\n", nextState, nextC, nextCharNum);

		

		if (nextState > 999) // Token complete
		{
			tokLen++;

			// Add last char to str
			strHEAD = (char *) realloc(strHEAD, sizeof(char) * (tokLen + 1));
			if (strHEAD == NULL)
			{
			    fprintf(stderr, "Failed to allocate memory for token instance.\n");
			    exit(-1);
			}
			*str++ = (char) c;
			*str = '\0';

			// Allocating memory for token
			tokenType* token = malloc(sizeof(tokenType));
			if (token == NULL)
			{
		        fprintf(stderr, "Failed to allocate memory for token.\n");
		        exit(-1);
	    	}
	    	token->tokenID = nextState;
	    	if (token->tokenID == 1022)
				tokLen++;

			token->lineNum = lineNumber;

			token->tokenInstance = strHEAD;
			if (token->tokenID == 1001)
				token->tokenID = testIfKeyword(token->tokenInstance);

			return token;
		}
		else if(nextState < 0) // Error
		{
			fprintf(stderr, "%s\n", returnStateID(nextState));
			exit(-1);
		}
		else
		{
			if(charNum == 0 || charNum == 28)
			{
				// Skip whitespace
				state = 0;
			}
			else if(charNum == 26)
			{	
				// Count new lines
				lineNumber++;
				state = 0;
			}
			else
			{
				tokLen++;
				strHEAD = (char *) realloc(strHEAD, sizeof(char) * (tokLen + 1));
				if (strHEAD == NULL)
				{
			        fprintf(stderr, "Failed to allocate memory for token instance.\n");
			        exit(-1);
			    }
			    *str++ = (char) c;
			    // state = nextState;
			}
		}
	}

	// EOF Tok
	tokenType* token = malloc(sizeof(tokenType));
	if (token == NULL)
	{
	    fprintf(stderr, "Failed to allocate memory for token.\n");
	    exit(-1);
	}
	token->tokenID = 1000;
	token->lineNum = lineNumber;
	// No token instance
	free(strHEAD);

	return token;
}

int testIfKeyword(char* id)
{
	if (strcmp(id, "Begin") == 0)
		return 2000;
	else if (strcmp(id, "End") == 0)
		return 2001;
	else if (strcmp(id, "Check") == 0)
		return 2002;
	else if (strcmp(id, "Loop") == 0)
		return 2003;
	else if (strcmp(id, "Void") == 0)
		return 2004;
	else if (strcmp(id, "Var") == 0)
		return 2005;
	else if (strcmp(id, "Return") == 0)
		return 2006;
	else if (strcmp(id, "Input") == 0)
		return 2007;
	else if (strcmp(id, "Output") == 0)
		return 2008;
	else if (strcmp(id, "Program") == 0)
		return 2009;
	else
		return 1001;
}

int determineCharNum(char str)
{
	if (str == ' ')
			return 0;
		else if (isalpha(str))
			return 1;
		else if (isdigit(str))
			return 2;
		else if (str == '_')
			return 3;
		else if (str == EOF)
			return 4;
		else if (str =='=')
			return 5;
		else if (str == '<')
			return 6;
		else if (str == '>')
			return 7;
		else if (str == '!')
			return 8;
		else if (str == ':')
			return 9;
		else if (str == '+')
			return 10;
		else if (str == '-')
			return 11;
		else if (str == '*')
			return 12;
		else if (str == '/')
			return 13;
		else if (str == '&')
			return 14;
		else if (str == '%')
			return 15;
		else if (str == '.')
			return 16;
		else if (str == '(')
			return 17;
		else if (str == ')')
			return 18;
		else if (str == ',')
			return 19;
		else if (str == '{')
			return 20;
		else if (str == '}')
			return 21;
		else if (str == ';')
			return 22;
		else if (str == '[')
			return 23;
		else if (str == ']')
			return 24;
		else if (str == '#')
			return 25;
		else if (str == '\n')
			return 26;
		else if (str == '\0')
			return 27;
		else if (str == '\t')
			return 28;
		else // Other
			return 29;
}

/*
	ID_UNDRSCR_ERR 	= -1,
	ID_DGGT_ERR 	= -2,
	BNG_ERR		= -3,
	CMMNT_ERR	= -4,
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
	CMMTOK		= 1016,
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
