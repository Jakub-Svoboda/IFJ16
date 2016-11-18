#include <stdio.h>
#include <stdlib.h>
#include "garbage.h"

#ifndef	ILIST_H
#define ILIST_H 

//types of instructions
#define I_STOP    	0		//end of program
#define I_LABEL		1		//jump label
#define I_GOTO		2		//goto a label
#define I_NEW_VAR	3
#define I_FN_END	4		//end of a function body. If there was no return, this will be executed.
#define I_RETURN	5		//If first arg is NULL, no expr was returned -> return;
#define I_IF_GOTO	6		//conditional jump
#define I_FN_CALL	7		//goto a function label
#define I_PROGRAM	8		//first instr		
#define I_MOV_INT	9		//move int CONSTANT number to adr1
#define I_MOV_DOUBLE	10
#define I_MOV_STRING	11
#define	I_ADD			12
#define	I_SUB			13
#define	I_MUL			14
#define	I_DIV			15
#define	I_LT			16
#define	I_GT			17
#define	I_LE			18
#define	I_GE			19
#define	I_EQ			20
#define	I_NE			21
#define I_MOV			22
#define I_WHILE_GOTO	23	//cond jump to while statement (jump if not zero)
#define I_RETURN_NOTHING	24	//return;
#define I_PUSH				25	//signals a argument: type, name
#define	I_CLEAR_TMPS		26	//clears a local var table of temporary variables used for expressions			
#define I_RETURN_MOV		27	//where returned variable has to be assigned
#define I_GLOBAL_PRE		28	//upper wrapper of global variable declaration and expression
#define I_GLOBAL_POST		29	//bottom wrapper of global variable declaration and expression
#define I_CLASS				30	//class label
#define I_READ_INT			31	//builtin functions
#define I_READ_STRING		32	
#define I_READ_DOUBLE		33
#define I_PRINT				34
#define	I_LENGTH			35
#define I_SUBSTR			36
#define I_COMPARE			37
#define I_FIND				38
#define I_SORT				39

typedef struct		//List of instructions
{
  struct listItem *first;  // pointer to first element
  struct listItem *last;   // pointer to last element
  struct listItem *active; // pointer to active element
} tListOfInstr;

typedef struct{	//tInstr contains type of instruction and three adresses for TAC
  int instType;  
  char addr1[2047]; 
  char addr2[2047]; 
  char addr3[2047]; 
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
#endif