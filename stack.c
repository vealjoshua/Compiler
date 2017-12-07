// C program for linked list implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
 
void push(StackNode** root, tokenType* token)
{
    StackNode* stackNode = newNode(token);
    stackNode->next = *root;
    *root = stackNode;
    printf("%s pushed to stack\n", token->tokenInstance);
}
 
char* pop(StackNode** root)
{
    if (isEmpty(*root))
        return NULL;
    StackNode* temp = *root;
    *root = (*root)->next;
    char* popped = malloc(sizeof(char) * ((strlen(temp->token->tokenInstance) + 1)));
    strcpy(popped, temp->token->tokenInstance);
    free(temp);
    printf("%s popped from stack\n", popped);

    return popped;
}

void popStack(StackNode** root) // Does not work
{
    if (pop(root))
        popStack(root);
}

int find(StackNode* root, char* idTok, int distance)
{
    if (root)
    {
        if (strcmp(root->token->tokenInstance, idTok) == 0)
        {
            return distance;
        }
        else
            find(root->next, idTok, --distance);
    }
    return -1;
}
 
char* peek(StackNode* root)
{
    if (isEmpty(root))
        return NULL;
    return root->token->tokenInstance;
}
