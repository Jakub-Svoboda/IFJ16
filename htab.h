#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTAB_SIZE 5

typedef struct thtabItem{
	const char* key;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(const char* key);
void htabInit(thtabItem* htab[]);
thtabItem* htabSearch(thtabItem* htab[], const char* key);
void htabInsert(thtabItem* htab[], const char* key);
const char* htabRead(thtabItem* htab[], const char* key);
void htabDelete(thtabItem* htab[], const char* key);
void htabDispose(thtabItem* htab[]);
