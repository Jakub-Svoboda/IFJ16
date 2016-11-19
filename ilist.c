/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    ilist.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "ilist.h"

// Initialize the list of instructions
void listInit(tListOfInstr *L){
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}

void listInsertLast(tListOfInstr *L, tInstr I){
	tListItem *newItem;
	newItem = memalloc(sizeof (tListItem));
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
	if (L->active != NULL &&  L->active->nextItem !=NULL)
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
void *listGetPointerLast(tListOfInstr *L){
  return (void*) L->last;
}

void listPrint(tListOfInstr *L){
	listFirst(L);
	while(1){
		if(L->last != L-> active){
			fprintf(stderr,"instruction: %d\n",L->active->Instruction.instType);
			listNext(L);
		}else{
			fprintf(stderr,"instruction: %d\n",L->active->Instruction.instType);
			break;
		}			
	}	
}