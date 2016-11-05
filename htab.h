#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define HTAB_SIZE 5
//#define thTable thtabItem*

typedef struct thtabItem{
	const char* key;
	Token_type returnType;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(const char* key);
void htabInit(thTable *htab);
thtabItem* htabSearch(thTable *htab, const char* key);
void htabInsert(thTable *htab, const char* key);
//const char* htabRead(thtabItem* htab[], const char* key);
void htabDelete(thTable *htab, const char* key);
void htabDispose(thTable *htab);
