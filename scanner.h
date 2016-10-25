typedef enum{
		token_identifier,
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
    //RelOp
    token_equal,
    token_greater,
    token_less,
    token_notEqual,
    token_greaterEqual,
    token_lessEqual,
    //ArOp
    token_add,
    token_subtract,
    token_multiply,
    token_divide,
    //Delimiters
    token_dot,
    token_bracketLeftRound,
    token_bracketRightRound,
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

typedef enum {
	state_default,
	state_readingNumber,
	state_readingIdentifier,
	state_readingString,
} State_type;

typedef struct _Token {
    Token_type type;
    //ptr hashtable
} Token;
