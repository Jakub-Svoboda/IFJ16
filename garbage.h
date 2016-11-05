#include <stdio.h>
#include <stdlib.h>

typedef struct tElem {			//List node
    struct tElem *next;			//Pointer to the next node
    void *memptr;			//Pointer to the allocated memory
} *tElemPtr;	               

typedef struct { 			//List
    tElemPtr First;			//Pointer to the first node
} tList;

extern tList A;			//Global variable which stores the pointers to the allocated memory

void *memalloc(unsigned);			//Allocates memory and stores the pointer to that memory
void memfree();			//Dispose list and free memory