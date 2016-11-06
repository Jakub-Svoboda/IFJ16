#include "iList.h"

// Initialize the list of instructions
void listInit(tListOfInstr *L){
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}

void listInsertLast(tListOfInstr *L, tInstr I){
	tListItem *newItem;
	newItem = malloc(sizeof (tListItem));
	newItem->Instruction = I;
	newItem->nextItem = NULL;
	if (L->first == NULL){
		L->first = newItem;
	}
	else{
		L->last->nextItem=newItem;
	}
	L->last=newItem;
}

void listFirst(tListOfInstr *L){ //First instruction becomes active
	L->active = L->first;
}

void listNext(tListOfInstr *L){	//Moves the activity to next element
	if (L->active != NULL)
	L->active = L->active->nextItem;
}

tInstr *listGetData(tListOfInstr *L){	//Returns the active instruction
	if (L->active == NULL){
		fprintf(stderr,"Instruction data requested but no instruction is active.");
		return NULL;
	}else{
		return &(L->active->Instruction);
	}
}