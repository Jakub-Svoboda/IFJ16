/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    precedence.h
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#define MAX_STACK 1023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef PRECEDENCE_H
#define PRECEDENCE_H


typedef struct{
	Token *arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

Token* getModifiedTokenPrecedence(FILE *f,Token* tokenPtr);
void stackPush(tStack *s,Token *Token);
void stackPop(tStack* s);
Token* stackTop(tStack *s);
int stackEmpty(tStack* s);
char* runPrecedenceAnalysis(FILE* f,Token* tokenPtr,int readFirst, tListOfInstr * list);
Token* reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack,  tListOfInstr * list);
Token* whatRule(tStack* stack,tListOfInstr * list);
void printStack(tStack* s);
Token* stackTopTerminal(tStack* s);


#endif