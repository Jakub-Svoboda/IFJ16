/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    syntax.h
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "ilist.h"
#include "ial.h"
#include "precedence.h"
#include "builtin.h"


#ifndef	SYNTAX_H
#define SYNTAX_H
#include "garbage.h"

#define argInt	28
#define argDouble	23
#define argString 30

void getModifiedToken(FILE *f,Token* tokenPtr);
int runSyntaxAnalysis (FILE *f, tListOfInstr * list);
int syntaxCheck (int state, FILE *f,Token* tokenPtr,Token* lastToken, tListOfInstr * list);
void generateInstruction(tInstr I, int instType, char addr1[2047], char addr2[2047], char addr3[2047],tListOfInstr* list);
int builtin(FILE *f, Token * tokenPtr, tListOfInstr * list,char * var);

typedef enum state{			//states for checkSyntax function
	CLASS_BLOCK,
	CLASS,
	CLASS_NAME,
	LEFT_CURLY_BRACKET,
	RIGHT_CURLY_BRACKET,
	STATIC_DECL,
	ID,
	CLASS_BODY,
	TYPE,
	ASSIGN,
	RIGHT_CURLY_BRACKET_CURRENT,
	FUNCTION_DECLARE,
	ID_CURRENT,
	TYPE_CURRENT,
	FN_BODY,
	FN_BODY_BEGIN,
	SEMICOLON,
	CLASS_BODY_CURRENT,
	FN_BODY_CURRENT,
	LOCAL_VAR_DEC,
	FN_CALL,
	LEFT_ROUND,
	RIGHT_ROUND,
	COMMAND_BLOCK_BEGIN,
	COMMAND_BLOCK,
	ELSE,
	BREAK,
	FN_CALL_COMMA

}	State;


typedef struct{			//a structure that the global pointer is pointing to
	thTable* functionTable;		//table for functions needs to be accessable globally
	thTable* globalVarTable;	//table for global variables needs to be accessable globally
	Token* classPtr;			//pointer to current class. Required for proper label generating.
	Token* funcPtr;				//pointer to current function. Required for proper label generating.	
	Token *callPtr;				//pointer to name of called function. Needs to be accessed from recursive call.
	tList* memList;				//A garbage collector list of allocated memory. 
}resourceStruct;



#endif
