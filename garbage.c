#include "garbage.h"

tList A;			//Global variable for storing pointers to memory

void *memalloc(unsigned size) {			//Function allocates memory given and stores poiter to that memory
	tElemPtr hPtr = malloc(sizeof(struct tElem));			//Allocating memory for new list node
	hPtr->memptr = malloc(size);			//Allocating memory given
	hPtr->next = A.First;			//Set new node next pointer to the first node
	A.First = hPtr;			//First is now new node

	return(A.First->memptr);			//Return pointer to the newly allocated memory
}

void listdisp() {			//Dispose pointer list and free all memory
	tElemPtr hPtr;			//Help pointer

	while (A.First != NULL) {			//While the list is not empty
		hPtr = A.First;			//Help points to the first node
		A.First = A.First->next;			//First now points to the next node
		free(hPtr->memptr);			//Free help memptr memory
		free(hPtr);			//Free help node memory
	}
}

/*int main(int argc, char **argv) {
	void *memptr = NULL;

	memptr = memalloc(sizeof(int));
	printf("%p\n", memptr);
	memptr = memalloc(sizeof(int));
	printf("%p\n", memptr);
	memptr = memalloc(sizeof(int));
	printf("%p\n", memptr);

	listdisp();

	return(0);
}*/