#include "token.h"

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
		case LSSREQTOK:
			return "LSSREQTOK";
		case GRTREQTOK:
			return "GRTREQTOK";
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
