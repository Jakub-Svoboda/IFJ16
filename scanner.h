#include <stdio.h>			//TODO check if this is ok or BS

typedef enum{

//		token_identifier,
//    token_invalid,
//    token_assign,
//    token_EOF,

	//ArOp
	token_add,			//0
	token_subtract,			//1
    token_multiply,			//2
    token_divide,			//3
	//RelOp
	token_less,			//4
    token_greater,			//5
    token_equal,			//6
	token_lessEqual,		//7
    token_greaterEqual,			//8
    token_notEqual,			//9
	token_bracketLeftRound,		//10
    token_bracketRightRound,		//11
	token_identifier,		//12
	token_dollar,			//13	Kuba added this
	token_expression,		//14	Kubba added this, not sure if needed?


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
    token_bracketRightCurly,		//38
    token_semicolon,			//39
    token_bracketLeftSquare,		//40
    token_bracketRightSquare,		//41
    //"" ''
    token_quotesSingle,			//42
    token_quotesDouble,			//43
    token_string,			//44
    token_intNumber,			//45
    token_doubleNumber,			//46

	token_leftHandle,		//47	Kuba added this
	token_rightHandle		//48	Kuba added this
} Token_type;


typedef enum {
	state_default,
	state_readingNumber,
	state_readingIdentifier,
	state_readingString,
} State_type;

typedef struct _Token {
    Token_type type;
//typedef struct {				//Kuba-edit
//    Token_type type;
    //ptr hashtable
} Token;

typedef struct _List_Elem {                 /* prvek dvousměrně vázaného seznamu */
        Token token;                                            /* užitečná data */
        struct _List_Elem *prev;          /* ukazatel na předchozí prvek seznamu */
        struct _List_Elem *next;        /* ukazatel na následující prvek seznamu */
} *Token_List_Elem;

typedef struct _Token_List{                                  /* dvousměrně vázaný seznam */
    Token_List_Elem first;                      /* ukazatel na první prvek seznamu */
    Token_List_Elem act;                     /* ukazatel na aktuální prvek seznamu */
    Token_List_Elem last;                    /* ukazatel na posledni prvek seznamu */
} Token_List;


Token *getToken(FILE *f);
Token *tokenInit();
