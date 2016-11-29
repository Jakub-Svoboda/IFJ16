/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    scanner.h
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include <stdio.h>
#include "garbage.h"



#ifndef SCANNER_H
#define SCANNER_H
#define KEYWORD_OFFSET 18;  //Change if more types added before token_boolean
#define BUFFER_SIZE 32;
typedef enum{
	//ArOp
	token_add,				//0
	token_subtract,			//1
    token_multiply,			//2
    token_divide,			//3
	//RelOp
	token_less,				//4
    token_greater,			//5
    token_equal,			//6
	token_lessEqual,		//7
    token_greaterEqual,		//8
    token_notEqual,			//9
	token_bracketLeftRound,		//10
    token_bracketRightRound,	//11
	token_identifier,		//12
	token_dollar,			//13
	token_expression,		//14

    token_invalid,			//15
    token_assign,			//16
    token_EOF,				//17
    //Keywords
    token_boolean,			//18
    token_break,			//19
    token_class,			//20
    token_continue,			//21
    token_do,				//22
    token_double,			//23
    token_else,				//24
    token_false,			//25
    token_for,				//26
    token_if,				//27
    token_int,				//28
    token_return,			//29
    token_String,			//30
    token_static,			//31
    token_true,				//32
    token_void,				//33
    token_while,			//34

    //Delimiters
    token_dot,				//35
    token_comma,			//36
    token_bracketLeftCurly,		//37
    token_bracketRightCurly,	//38
    token_semicolon,			//39
    token_bracketLeftSquare,	//40
    token_bracketRightSquare,	//41
    //"" ''
    token_quotesSingle,		//42
    token_quotesDouble,		//43
    token_string,			//44
    token_intNumber,		//45
    token_doubleNumber,		//46

	token_leftHandle,		//47
	token_rightHandle		//48
} Token_type;



typedef struct {
    Token_type type;
    char* name;
} Token;

typedef enum {
	state_default,
	state_readingNumber,
	state_readingIdentifier,
	state_readingString
} State_type;

Token *getToken(FILE *f);
Token *tokenInit();
Token *lookAhead(FILE *f, int steps);  // Choose 0/1 steps ahead
#endif
