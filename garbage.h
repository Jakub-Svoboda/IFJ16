#include <stdio.h>
#include <stdlib.h>



#ifndef	GARBAGE_H
#define GARBAGE_H


typedef struct tElem {			//List node
    struct tElem *next;			//Pointer to the next node
    void *memptr;			//Pointer to the allocated memory
} *tElemPtr;	               

typedef struct { 			//List
    tElemPtr First;			//Pointer to the first node
} tList;

void *memalloc(unsigned int);			//Allocates memory and stores the pointer to that memory
void memfreeall();			//Frees all allocated memory
void memfree(void *);			//Frees memory on pointer specified
void *memrealloc(void *ptr, unsigned int size);


#include "syntax.h"			//TODO check sometimes
#endif