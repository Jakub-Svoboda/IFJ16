#include "garbage.h"

tList A;			//Global variable for storing pointers to memory

void *memalloc(unsigned size) {			//Function allocates memory given and stores poiter to that memory
	tElemPtr hPtr = malloc(sizeof(struct tElem));			//Allocating memory for new list node
	hPtr->memptr = malloc(size);			//Allocating memory given
	hPtr->next = A.First;			//Set new node next pointer to the first node
	A.First = hPtr;			//First is now new node

	return(A.First->memptr);			//Return pointer to the newly allocated memory
}

void memfreeall() {			//Dispose pointer list and free all memory
	tElemPtr hPtr;			//Help pointer

	while (A.First != NULL) {			//While the list is not empty
		hPtr = A.First;			//Help points to the first node
		A.First = A.First->next;			//First now points to the next node
		free(hPtr->memptr);			//Free help memptr memory
		free(hPtr);			//Free help node memory
	}
}

void memfree(void *ptr) {
	tElemPtr hPtr;
	tElemPtr ttPtr = A.First;
	tElemPtr tPtr = A.First;

	if(tPtr->memptr == ptr) {
		hPtr = tPtr;
		tPtr = tPtr->next;
		free(hPtr->memptr);
		free(hPtr);
		hPtr = NULL;
		A.First = tPtr;
		ttPtr = NULL;
		tPtr = NULL;
		return;
	}
	tPtr = tPtr->next;

	while(tPtr != NULL) {
		
		if(tPtr->memptr != ptr) {
			tPtr = tPtr->next;
			ttPtr = ttPtr->next;
		}
		else {
			hPtr = tPtr;
			tPtr = tPtr->next;
			ttPtr->next = tPtr;
			free(hPtr->memptr);
			free(hPtr);
			hPtr = NULL;
			ttPtr = NULL;
			tPtr = NULL;
		}
	}
}