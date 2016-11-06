#include <stdio.h>
#include <stdlib.h>

typedef struct tElem {			//List node
    struct tElem *next;			//Pointer to the next node
    void *memptr;			//Pointer to the allocated memory
} *tElemPtr;	               

typedef struct { 			//List
    tElemPtr First;			//Pointer to the first node
} tList;

void *memalloc(unsigned, tList *);			//Allocates memory and stores the pointer to that memory
void memfreeall(tList *);			//Frees all allocated memory
void memfree(void *, tList *);			//Frees memory on pointer specified