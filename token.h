#ifndef TOKEN_H
#define TOKEN_H

enum token_enum
{
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
	LSSREQTOK	= 1024,
	GRTREQTOK	= 1025,
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
};

typedef struct tokenType
{
	enum token_enum tokenID;
	char* tokenInstance;
	int lineNum;
} tokenType;

char* returnStateID(enum token_enum state);

#endif
