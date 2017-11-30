// C program for linked list implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include "stack.h" 
 
StackNode* newNode(tokenType* token)
{
    StackNode* stackNode =
              (StackNode*) malloc(sizeof(StackNode));
    stackNode->token = token;
    stackNode->next = NULL;
    return stackNode;
}
 
int isEmpty(StackNode* root)
{
    return !root;
}
 
void push(StackNode* root, tokenType* token)
{
    StackNode* stackNode = newNode(token);
    stackNode->next = root;
    root = stackNode;
    printf("%s pushed to stack\n", token->tokenInstance);
}
 
char* pop(StackNode* root)
{
    if (isEmpty(root))
        return NULL;
    StackNode* temp = root;
    root = root->next;
    char* popped = temp->token->tokenInstance;
    free(temp);
 
    return popped;
}

int find(StackNode* root, char* idTok, int distance)
{
    return (strcmp(root->token->tokenInstance, idTok) == 0 || distance < 0)? distance: find(root, idTok, --distance);
}
 
char* peek(StackNode* root)
{
    if (isEmpty(root))
        return NULL;
    return root->token->tokenInstance;
}
