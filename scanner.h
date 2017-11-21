#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

tokenType** scanner(char* str, int lineNumber, int* numOfTokens);
int testIfKeyword(char* id);
int determineCharNum(char str);

#endif
