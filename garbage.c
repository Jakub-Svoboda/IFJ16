#include "garbage.h"
extern resourceStruct *resources;

void *memalloc(unsigned int size) {			//Function allocates memory given and stores poiter to that memory
	tElemPtr hPtr = malloc(sizeof(struct tElem));			//Allocating memory for new list node
	hPtr->memptr = malloc(size);
	hPtr->next = resources->memList->First;
	resources->memList->First = hPtr;			//First is now new node
	return(resources->memList->First->memptr);			//Return pointer to the newly allocated memory
}

void memfreeall() {			//Dispose pointer list and free all memory
	tElemPtr hPtr;			//Help pointer

	while (resources->memList->First != NULL) {			//While the list is not empty
		hPtr = resources->memList->First;			//Help points to the first node
		resources->memList->First = resources->memList->First->next;			//First now points to the next node
		free(hPtr->memptr);			//Free help memptr memory
		free(hPtr);			//Free help node memory	
	}
	free(resources->memList);
	free(resources);
}

void *memrealloc(void *ptr, unsigned int size)
{
	tElemPtr hPtr = memalloc(sizeof(struct tElem));
	hPtr = resources->memList->First;
	while(1){
		hPtr=hPtr->next;
		if(ptr==hPtr->memptr)
			break;
	}
	hPtr->memptr=realloc(hPtr->memptr,size);
	return hPtr->memptr;	
}

void memfree(void *ptr) {			//Function frees memory on specified pointer (must be allocated using memalloc, NOT malloc)
	tElemPtr hPtr;			//Help pointer
	tElemPtr ttPtr = resources->memList->First;			//Pointer to previous node
	tElemPtr tPtr = resources->memList->First;			//Pointer to current node

	if(tPtr->memptr == ptr) {		//If the pointer given as argument equeals memptr pointer in the current (First in this case) node 
		hPtr = tPtr;			//Backup current node
		tPtr = tPtr->next;			//Current node moves to the next node
		free(hPtr->memptr);			//Frees memory allocated on the pointer given as argument
		free(hPtr);			//Frees the node in which the pointer given as argument was
		hPtr = NULL;
		resources->memList->First = tPtr;			//First node is now the next node
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