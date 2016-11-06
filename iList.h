#include <stdio.h>
#include <stdlib.h>

//types of instructions
#define I_STOP    	0		//end of program
#define I_LABEL		1		//jump label

typedef struct		//List of instructions
{
  struct listItem *first;  // pointer to first element
  struct listItem *last;   // pointer to last element
  struct listItem *active; // pointer to active element
} tListOfInstr;

typedef struct{	//tInstr contains type of instruction and three adresses for TAC
  int instType;  
  void *addr1; 
  void *addr2; 
  void *addr3; 
} tInstr;

typedef struct listItem{		//List item contains an instruction and a pointer to the next item
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;

void listInit(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstr I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
tInstr *listGetData(tListOfInstr *L);
void listPrint(tListOfInstr *L);
void *listGetPointerLast(tListOfInstr *L);