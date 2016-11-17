#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"



#ifndef	HTAB_H
#define HTAB_H
#define HTAB_SIZE 5
#define UNINIT 0
#define INIT 1
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
