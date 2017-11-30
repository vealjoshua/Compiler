#ifndef STACK_H
#define STACK_H

#include "token.h"

// A structure to represent a stack
struct StackNode
{
    tokenType* token;
    struct StackNode* next;
} typedef StackNode;

StackNode* newNode(tokenType* token);
int isEmpty(StackNode* root);
void push(StackNode* root, tokenType* token);
char* pop(StackNode* root);
int find(StackNode* root, char* idTok, int distance);
char* peek(StackNode* root);

#endif