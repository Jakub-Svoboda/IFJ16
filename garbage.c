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

void memfree(void *ptr) {			//Function frees memory on specified pointer (must be allocated using memalloc, NOT malloc)
	tElemPtr hPtr;			//Help pointer
	tElemPtr ttPtr = A.First;			//Pointer to previous node
	tElemPtr tPtr = A.First;			//Pointer to current node

	if(tPtr->memptr == ptr) {		//If the pointer given as argument equeals memptr pointer in the current (First in this case) node 
		hPtr = tPtr;			//Backup current node
		tPtr = tPtr->next;			//Current node moves to the next node
		free(hPtr->memptr);			//Frees memory allocated on the pointer given as argument
		free(hPtr);			//Frees the node in which the pointer given as argument was
		hPtr = NULL;
		A.First = tPtr;			//First node is now the next node
		ttPtr = NULL;
		tPtr = NULL;
		return;
	}
	tPtr = tPtr->next;			//Current node moves to the next node

	while(tPtr != NULL) {			//While not the end of list
		
		if(tPtr->memptr != ptr) {			//If the pointer given as argument does not equeal memptr pointer in the current node
			tPtr = tPtr->next;			//Current node moves to the next node
			ttPtr = ttPtr->next;			//Previous node moves to the next node
		}
		else {
			hPtr = tPtr;			//Backup current node
			tPtr = tPtr->next;			//Current node moves to the next node
			ttPtr->next = tPtr;			//Set pointer next of previous node to the current node
			free(hPtr->memptr);			//Frees memory allocated on the pointer given as argument
			free(hPtr);			//Frees the node in which the pointer given as argument was
			hPtr = NULL;
			ttPtr = NULL;
			tPtr = NULL;
		}
	}
}