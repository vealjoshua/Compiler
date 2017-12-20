#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

void ADD(char* arg);
void BR(char* arg, int funcNum);
void BRNEG(char* arg, int funcNum);
void BRZNEG(char* arg, int funcNum);
void BRPOS(char* arg, int funcNum);
void BRZPOS(char* arg, int funcNum);
void BRZERO(char* arg, int funcNum);
void COPY(char* arg1, char* arg2);
void DIV(char* arg);
void FUNC(char* arg, int funcNum);
void GLOBVARS(char* gv);
void LOAD(char* arg);
void MULT(char* arg);
void NOOP();
void POP();
void PUSH();
void READ(char* arg);
void STACKR(int arg);
void STACKW(int arg);
void STOP();
void STORE(char* arg);
void SUB(char* arg);
void WRITE(char* arg);

#endif