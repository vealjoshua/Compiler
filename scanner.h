#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

tokenType* scanner();
int testIfKeyword(char* id);
int determineCharNum(char str);
char* returnStateID(enum token_enum state);

#endif
