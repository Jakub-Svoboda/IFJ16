#define MAX_STACK 1023
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"


typedef struct{
	Token *arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

void stackPush(tStack *s,Token *Token);
void stackPop(tStack* s);
void stackTop(tStack *s, Token *Token);
int stackEmpty(tStack* s);
int runPrecedenceAnalysis();
void reduction(char c[]);