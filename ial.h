/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    ial.h
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#ifndef	IAL_H
#define IAL_H
#define HTAB_SIZE 5
#define UNINIT 0
#define INIT 1

void partition(char *, int, int, int *, int *);
void quick_sort(char *, int, int);

int min(int, int);
int max(int, int);
void compute_jumps(char *, int *);
void compute_match_jump(char *, int *);
int bma(char *, char *, int *, int *);






//#define thTable thtabItem*

typedef struct
{
	int argumentType;
	char* argumentName;
}argStruct;


typedef struct thtabItem{
	char* key;
	char* name;
	char* classKey;
	Token_type returnType;
	Token_type varType;
	struct thtabItem* next;
	int intValue;
	double doubleValue;
	char* stringValue;
	int boolValue;
	int isInit;
	int argumentNumber;
	argStruct* arguments[127];
	
} thtabItem;



typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(char* key);
void htabInit(thTable *htab);
thtabItem* htabSearch(thTable *htab, char* key);
void htabDeleteHashtag(thTable *htab);
char* htabSearchHashtag(thTable *htab);
thtabItem* htabSearchClass(thTable *htab, char* key, char* classKey);
void htabInsert(thTable *htab, char* key, int localVarType);
//const char* htabRead(thtabItem* htab[], char* key);
void htabDelete(thTable *htab, char* key);
void htabDispose(thTable *htab);
void htabInsertReturnType(thTable *htab, char* funcName, char* classKey, Token_type ret);
void htabInsertVarType(char* varName, char* classKey, Token_type var);
void printHtab(thTable *htab, int var);
void printHtabLocal(thTable *htab);
char* concat(char* str1, char* str2);
#endif