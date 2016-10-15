#include <stdio.h>			//TODO check if this is ok or BS

typedef enum{
	//ArOp
	token_add,
	token_subtract,
    token_multiply,
    token_divide,	

	//RelOp
	token_less,
    token_greater,
    token_equal,
	token_lessEqual,
    token_greaterEqual,
    token_notEqual,
	token_bracketLeftRound,
    token_bracketRightRound,
	token_identifier,
	token_dollar,				//Kuba added this

	
    token_invalid,
    token_assign,
    token_EOF,
    //Keywords
    token_boolean,
    token_break,
    token_class,
    token_continue,
    token_do,
    token_double,
    token_else,
    token_false,
    token_for,
    token_if,
    token_int,
    token_return,
    token_String,
    token_static,
    token_true,
    token_void,
    token_while,

    //Delimiters
    token_dot,
    token_comma,
    token_bracketLeftCurly,
    token_bracketRightCurly,
    token_semicolon,
    token_bracketLeftSquare,
    token_bracketRightSquare,
    //"" ''
    token_quotesSingle,
    token_quotesDouble,
    token_string,
    token_intNumber,
    token_doubleNumber
	
} Token_type;



typedef struct {				//Kuba-edit
    Token_type type; 
    //ptr hashtable
} Token;



Token *getToken(FILE *f);
Token *tokenInit();

