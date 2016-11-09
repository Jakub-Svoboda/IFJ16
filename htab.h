#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "iList.h"

#define HTAB_SIZE 5
//#define thTable thtabItem*

typedef struct thtabItem{
	char* key;
	char* name;
	char* classKey;
	Token_type returnType;
	Token_type varType;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(char* key);
void htabInit(thTable *htab);
thtabItem* htabSearch(thTable *htab, char* key);
thtabItem* htabSearchClass(thTable *htab, char* key, char* classKey);
void htabInsert(thTable *htab, char* key);
//const char* htabRead(thtabItem* htab[], char* key);
void htabDelete(thTable *htab, char* key);
void htabDispose(thTable *htab);
void htabInsertReturnType(thTable *htab, char* funcName, char* classKey, Token_type ret);
void htabInsertVarType(thTable *htab, char* varName, char* classKey, Token_type var);
void printHtab(thTable *htab, int var);
void printHtabLocal(thTable *htab);
