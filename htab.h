#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTAB_SIZE 5

typedef struct {
	char* key;
//	int length;  needed?
	struct thtabItem* next;
} thtabItem;


typedef thtabItem* thTable[HTAB_SIZE];
