#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define HTAB_SIZE 5
//#define thTable thtabItem*

typedef struct thtabItem{
	char* key;
	//Token_type returnType;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(char* key);
void htabInit(thTable *htab);
thtabItem* htabSearch(thTable *htab, char* key);
void htabInsert(thTable *htab, char* key);
//const char* htabRead(thtabItem* htab[], char* key);
void htabDelete(thTable *htab, char* key);
void htabDispose(thTable *htab);
void htabInsertReturnType(thTable *htab, char* key, Token_type returnType);
