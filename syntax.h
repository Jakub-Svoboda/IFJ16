#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <string.h>


void getModifiedToken(FILE *f,Token* tokenPtr);
int runSyntaxAnalysis (FILE *f);
int syntaxCheck (int state, FILE *f,Token* tokenPtr,Token* lookAheadPtr);
void getModifiedLookAhead(FILE *f,Token* tokenPtr);

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
	BREAK
	
}	State;