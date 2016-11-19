/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    garbage.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "garbage.h"
extern resourceStruct *resources;

void *memalloc(unsigned int size) {			//Function allocates memory given and stores poiter to that memory

	if(size == 0) {
		return(NULL);
	}
	tElemPtr hPtr = malloc(sizeof(struct tElem));		//Allocating memory for new list node
	if(hPtr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		memfreeall();
		exit(99);
	}
	hPtr->memptr = malloc(size);			//Allocating memory specified
	if(hPtr->memptr == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		free(hPtr);
		memfreeall();
		exit(99);
	}
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
/*	tElemPtr hPtr = memalloc(sizeof(struct tElem));
	hPtr = resources->memList->First;
	while(1){
		hPtr=hPtr->next;
		if(ptr==hPtr->memptr)
			break;
	}
	hPtr->memptr=realloc(hPtr->memptr,size);
	return hPtr->memptr;*/

	if(size == 0) {
		return(NULL);
	}
	tElemPtr hPtr = resources->memList->First;			//Help pointer
	tElemPtr tPtr = NULL;			//Help pointer

	while(hPtr != NULL) {

		if(hPtr->memptr == ptr) {			//If memory to be reallocated is found, realloc memory
			tPtr = realloc(hPtr->memptr, size);

			if(tPtr == NULL) {
				fprintf(stderr, "Memory reallocation failed\n");
				memfreeall();
				exit(99);
			}
			else {			//If reallocation was successful, return pointer to that memory
				hPtr->memptr = tPtr;
				return(hPtr->memptr);
			}
		}
		else {			//If memory to be allocated is not found in current node, move to the next node
			hPtr = hPtr->next;
		}
	}
	fprintf(stderr, "Nothing to realloc\n");			//If memory to be allocated is not found at all, print error and exit
	memfreeall();
	exit(99);
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