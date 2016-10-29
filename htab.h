#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTAB_SIZE 5

typedef struct thtabItem{
	char* key;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];

int hashFun(char* key);
void htabInit(thtabItem* htab[]);
thtabItem* htabSearch(thtabItem* htab[], char* key);
void htabInsert(thtabItem* htab[], char* key);
char* htabRead(thtabItem* htab[], char* key);
void htabDelete(thtabItem* htab[], char* key);
void htabDispose(thtabItem* htab[]);
//TODO


//void htClearAll ( tHTable* ptrht );
