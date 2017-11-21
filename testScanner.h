#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include <stdio.h>
#include "token.h"

void testScanner(FILE* fp);
char* returnStateID(enum token_enum state);

#endif
