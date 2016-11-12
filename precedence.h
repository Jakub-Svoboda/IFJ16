#define MAX_STACK 1023
#include <stdio.h>
#include <stdlib.h>
#include "firstRun.h"
#include <string.h>



typedef struct{
	Token *arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

Token* getModifiedTokenPrecedence(FILE *f,Token* tokenPtr);
void stackPush(tStack *s,Token *Token);
void stackPop(tStack* s);
Token* stackTop(tStack *s);
int stackEmpty(tStack* s);
int runPrecedenceAnalysis(FILE* f,Token* tokenPtr,int readFirst, tListOfInstr * list);
void reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack,  tListOfInstr * list);
Token* whatRule(tStack* stack,tListOfInstr * list);
void printStack(tStack* s);
Token* stackTopTerminal(tStack* s);