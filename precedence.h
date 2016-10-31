#define MAX_STACK 1023
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <string.h>



typedef struct{
	Token *arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

void getModifiedTokenPrecedence(FILE *f,Token* tokenPtr);
void stackPush(tStack *s,Token *Token);
void stackPop(tStack* s);
Token* stackTop(tStack *s);
int stackEmpty(tStack* s);
int runPrecedenceAnalysis(FILE* f,Token* tokenPtr);
void reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack);
int whatRule(tStack* stack);
void printStack(tStack* s);
Token* stackTopTerminal(tStack* s);