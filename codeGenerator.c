#include <stdio.h>
#include <string.h>
#include "extern.h"
#include "codeGenerator.h"

extern FILE* outputFile;

// Instruction List (# arguments, meaning)
/*
ADD, DIV, MULT, WRITE, LOAD, SUB can take either variable or immediate value as the arg:
immediate value is positive integer or negative integer
*/

/*
PUSH/POP are only means to reserve/delete automatic storage.
STACKW/STACKR n - these are stack write/read instructions.
n must be a non-negative number, and the access is to nth element down from TOS
NOTE: TOS points to the topmost element on the stack
*/

void ADD(char* arg) // (1, ACC = ACC +arg)
{
	fprintf(outputFile, "ADD %s\n", arg);
}

void BR(char* arg, int funcNum) // (1, jump to arg)
{
	fprintf(outputFile, "BR %s%d\n\n", arg, funcNum);
}

void BRNEG(char* arg, int funcNum) // (1, jump to arg if ACC <0)
{
	fprintf(outputFile, "BRNEG %s%d\n", arg, funcNum);
}

void BRZNEG(char* arg, int funcNum) // (1, jump to arg if ACC <=0)
{
	fprintf(outputFile, "BRZNEG %s%d\n", arg, funcNum);
}

void BRPOS(char* arg, int funcNum) // (1, jump to arg if ACC >0)
{
	fprintf(outputFile, "BRPOS %s%d\n", arg, funcNum);
}

void BRZPOS(char* arg, int funcNum) // (1, jump to arg if ACC >=0)
{
	fprintf(outputFile, "BRZPOS %s%d\n", arg, funcNum);
}

void BRZERO(char* arg, int funcNum) // (1, jump to arg if ACC ==0)
{
	fprintf(outputFile, "BRZERO %s%d\n", arg, funcNum);
}

void COPY(char* arg1, char* arg2) // (2, arg1 = arg2)
{
	fprintf(outputFile, "COPY %s %s\n", arg1, arg2);
}

void DIV(char* arg) // (1, ACC = ACC / arg)
{
	fprintf(outputFile, "DIV %s\n", arg);
}

void FUNC(char* arg, int funcNum)
{
	fprintf(outputFile, "%s%d: ", arg, funcNum);
}

void GLOBVARS(char* gv)
{
	fprintf(outputFile, "%s 0\n", gv);
}

void LOAD(char* arg) // (1, ACC=arg)
{
	fprintf(outputFile, "LOAD %s\n", arg);
}

void MULT(char* arg) // (1, ACC = ACC * arg)
{
	fprintf(outputFile, "MULT %s\n", arg);
}

void NOOP()  // (0, nothing)
{
	fputs("NOOP\n", outputFile);
}

void POP() // (0, tos++)
{
	fputs("POP\n", outputFile);
}

void PUSH() // (0, tos--)
{
	fputs("PUSH\n", outputFile);
}

void READ(char* arg) // (1, arg=input integer)
{
	fprintf(outputFile, "READ %s\n", arg);
}

void STACKR(int arg) // (1,ACC=stack[tos-arg])
{
	fprintf(outputFile, "STACKR %d\n", arg);
}

void STACKW(int arg) // (1,stack[tos-arg]=ACC)
{
	fprintf(outputFile, "STACKW %d\n", arg);
}

void STOP() // (0, stop program)
{
	fputs("STOP\n", outputFile);
}

void STORE(char* arg) // (1, arg = ACC)
{
	fprintf(outputFile, "STORE %s\n", arg);
}

void SUB(char* arg) // (1, ACC = ACC - arg)
{
	fprintf(outputFile, "SUB %s\n", arg);
}

void WRITE(char* arg) // (1, put arg to output as integer)
{
	fprintf(outputFile, "WRITE %s\n", arg);
}
