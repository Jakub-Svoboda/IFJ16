/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    syntax.h
* Date:    31.10.2016
* Team:    Michael Schmidt      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "precedence.h"


void getModifiedToken(FILE *f,Token* tokenPtr);
int runSyntaxAnalysis (FILE *f, tListOfInstr * list);
int syntaxCheck (int state, FILE *f,Token* tokenPtr,Token* lastToken, tListOfInstr * list);
void getModifiedLookAhead(FILE *f,Token* tokenPtr);
void generateInstruction(tInstr I, int instType, char addr1[2047], char addr2[2047], char addr3[2047],tListOfInstr* list);
thTable* enterScope();

typedef enum state{
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


typedef struct{
	thTable* functionTable;
	Token* classPtr;
	Token* funcPtr;
}resourceStruct;