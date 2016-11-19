#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
//#include "htab.c"

//#define memalloc malloc
//#define memrealloc realloc
#define true 1
#define false 0


char *keywords[] = {"boolean", "break", "class", "continue", "do", "double",
	"else", "false", "for", "if", "int", "return", "String", "static", "true","void","while"};

int isKeyword(char *string) {							//return index value of choosen keyword in array
    int i;
    for (i = 0; i < 17; i++) {
        if (!strcmp(keywords[i], string))
            return i;
    }
    return -1;											//if string is not keyword return -1
}

Token *tokenInit() {									//allocate space
	Token *t = (Token *)memalloc(sizeof(Token));		//ZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZ
	if(t == NULL) {
		printf("tokenInit malloc error\n");				//propably BS
		t->type = token_invalid;						//not sure if this is ok since there is only NULL in t, so it can't have ->type
	}
	t->type=-1;
	t->name = NULL;
	return t;
}

Token *lookAhead(FILE *f, int steps) { //TODO : Is there better way of passing FILE?
	//-------------------------------------------------------------------------------------------------
	//
	// I know it's not the same to call lookAhead(f,0) instead of getToken(f)so solution may be:
	//
	// A)
	//   -- #define getToken() lookAhead(f,0)
	//   -- #define lookAhead() lookAhead(f,1)
	// B)
	//	 -- using global static *nextTok but I thought globals are big NO NO
	// C)
	//	 -- deal with it
	//
	//-------------------------------------------------------------------------------------------------

/* USAGE:
*	Token *tempTok = lookAhead(f, 0);
*	Token *nextTok = lookAhead(f, 1);
*	tempTok = lookAhead(f, 0);
*	nextTok = lookAhead(f, 1);
*	tempTok = lookAhead(f, 0);
*	tempTok = lookAhead(f, 0);
*	tempTok = lookAhead(f, 0);
*	nextTok = lookAhead(f, 1);
*	tempTok = lookAhead(f, 0);
*/
	static Token *tNext;
	Token *t;
	switch(steps) {
		case 0:
			if(tNext == NULL) {			//lookAhead(1) hasn't been called last time, getToken is needed   ;;IDK if tenses are OK, bad english
				t = getToken(f);
			}else {						//lookAhead(1) has been called, return token that is already read
				t = tNext;
				tNext = NULL;			//NULL static token tNext because its value is currently returned
			}
			return t;
			break;
		case 1:
			//t = getToken(f);
			tNext = getToken(f);		//to look onto next token call lookAhead(1), it can't skip token, it can only look to next
			return tNext;
			break;
		default:
			break;
	}
	return NULL;
}

Token *getToken(FILE *f) { 	//TODO : Is there better way of passing FILE? 	//Call lookAhead instead of getToken();
							// Lex errors: id. is not ok
	int buffSize = BUFFER_SIZE;
	char *buff = (char*) memalloc(buffSize * sizeof(char));					//ZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZ

	char* name2 = memalloc(sizeof(char)*2);
	char* name3 = memalloc(sizeof(char)*3);
//	buffSize += BUFFER_SIZE;
//    buff = memrealloc(buff, buffSize);

	//printf("AYA");
	Token *t = tokenInit();	//TODO Kuba-edit
							// add to garbage collector?
//	char buff[1024];
    int c, position = 0, tempc = 0, kwIndex = 0, isDouble = 0, hasE = 0, isComplex = 0;		//tempc is buffered character, kwIndex is ord. value of keyword type, isDouble is boolean-like var
	State_type state = state_default;					//choose between states
	//t = malloc(sizeof(Token));
	//tokenInit(t);
	//t->type = token_invalid;
	if(t->type == token_invalid){						//initialization faiulure
		printf("Invalid token\n");
		return t;
	}


	while(1) {					//TODO ?
		c = fgetc(f);
		//printf("\nCHAR INPUT %c |||| ",c);
		switch (state) {								//check if I'm not in reading number/id/string phase
			case state_readingIdentifier:
				if(isalpha(c) || isdigit(c) || (c == '.' && isComplex==0) || c == '_'){			//id's may contain numbers and characters or 1 dot
					if(c == '.') isComplex = 1;
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
					    buff = memrealloc(buff, buffSize);
					}
				}else {									//end of allowed chars
					state = state_default;				//set state to default so we'll know we are not reading id anymore
					ungetc(c,f);						//back 1 char
				}
				if(state == state_default) {			//reading has ended
					buff[position] = '\0';				//add ending character at the end
					if((kwIndex = isKeyword(buff)) != -1) {		//if buffered word is in keyword array return kw token
						t->type = kwIndex + KEYWORD_OFFSET;		//value calculated by returned kwIndex and KEYWORD_OFFSET set in scanner.h
					}else {
						//TODO add buffered string to hashtable and link pointer
						t->name = buff;
						t->type = token_identifier;				//it's not keyword so return as id
						//printf("%s\n",buff);
					}
					position = 0;								//reset position of buffer
					isComplex = 0;								//reset isComplex
					//printf("%s  ",buff);
					return t;
				}
				break;
			case state_readingString:						//reading string now doesn't store ""
				if(c == EOF) {
					memfreeall();
					exit(1);
					//TODO: remove
					state = state_default;
					ungetc(c,f);
					buff[position] = '\0';
					t->type = token_invalid;
					position = 0;
					return t;
					//printf("%s ",buff);
				}else {
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
					if((c == '\"' && buff[position-2] != '\\')) {		//looking for " but only if previous char is not '\', so '\"' is not matching
						state = state_default;						//if found, end reading
					}
				}
				if(state == state_default) {			//same as readingIdentifier
					position--;							//I won't store '"'
					buff[position] = '\0';
					t->type = token_string;
					t->name = buff;
					//TODO add string into hashtable and link it to token
					//printf("'%s'",buff);				//remove when 100% working
					position = 0;
					return t;
				}
				break;
			case state_readingNumber:
				if(c == '.' && hasE == false && isDouble == false){
					isDouble = true;
				}else if((c == 'e' || c == 'E') && hasE == false) {	//when any character is <- , it is not average number, it is DOUBLE
					hasE = true;
				}else if(c == 'e' || c=='E' || c=='.'){
					isDouble = false;
				}

				if(isdigit(c)){							//not sure why I did if and else if,.. when it does the same thing
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else if(isDouble && (c == 'E' || c == 'e' || c == '+' || c == '-' || c == '.')){
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else {									//no lex error in case of s=3.14159+a because it is + id?
					ungetc(c,f);
					state = state_default;				//end reading state
					if(tempc == '+' || tempc == '-') {
						ungetc(tempc, f);
						position--;
					}
				}
				if(state == state_default) {
					buff[position] = '\0';
					if((isDouble) || (!isDouble && hasE)) {
						t->type = token_doubleNumber;
						t->name = buff;
						//TODO hashtable insert
						//printf("DABL'%s'\n",buff);		//shout out its double
					}else {
						t->type = token_intNumber;
						t->name = buff;
						//TODO hashtable insert
						//printf("INT'%s'",buff);			//its just number..
					}
					position = 0;						//reset
					isDouble = false;					//reset
					hasE = false;
					return t;
				}
				tempc = c;
				break;
			case state_default:									//this will be done at every beginning of token reading
				switch (c) {
					case ' ':									//skip trash like spaces, newlines and tabs
						break;
					case '\n':
						break;
					case '\t':
						break;
					//ArOp
					case '+':
						t->type = token_add;

						name2 = "+";
						t->name = name2;
						return t;
						break;
					case '-':
						t->type = token_subtract;

						name2 = "-";
						t->name = name2;
						return t;
					case '*':
						t->type = token_multiply;

						name2 = "*";
						t->name = name2;
						return t;
					//Division or Comments
					case '/':
						c = fgetc(f);
						if(c == '/') {							//line-comment found
							while((c=fgetc(f)) != '\n'){		//just skip it!
								if(c == EOF) {
									t->type = token_EOF;
									return t;
								}
								//printf("%c ",c);
							}

							break;
						}else if(c == '*') {					//block-comment found
							//printf("K");
							tempc = ' ';
							while(((c=fgetc(f)) != '/') || (tempc != '*')) {	//read until the combination of */ is found
								tempc = c;
								if(c == EOF) {
									t->type = token_EOF;
									return t;
								}
							}
							break;
						}else {									//next char is not matching any comment combination
							ungetc(c,f);						//move 1 char back and return token type
							t->type = token_divide;

							name2 = "/";
							t->name = name2;
							return t;
						}
					case '\'':									//TODO how to handle chars? does java require only "" for strings?
						break;
					case '\"':									//beggining of " had been found, set state to readingString and buffer the char
						state = state_readingString;
						//buff[position] = c;					// I won't store '"'
						//position++;								//increment position of buffer
						break;
					//Delimiters
					case '.':
						t->type = token_dot;

						name2 = ".";
						t->name = name2;
						return t;
					case '(':
						t->type = token_bracketLeftRound;

						name2 = "(";
						t->name = name2;
						return t;
					case ')':
						t->type = token_bracketRightRound;

						name2 = ")";
						t->name = name2;
						return t;
					case ',':
						t->type = token_comma;

						name2 = ",";
						t->name = name2;
						return t;
					case '{':
						t->type = token_bracketLeftCurly;

						name2 = "{";
						t->name = name2;
						return t;
					case '}':
						t->type = token_bracketRightCurly;

						name2 = "}";
						t->name = name2;
						return t;
					case ';':
						t->type = token_semicolon;

						name2 = ";";
						t->name = name2;
						return t;
					case '[':
						t->type = token_bracketLeftSquare;

						name2 = "[";
						t->name = name2;
						return t;
					case ']':
						t->type = token_bracketRightSquare;

						name2 = "]";
						t->name = name2;
						return t;
					//assign =, equal ==
					case '=':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_equal;

							name3 = "==";
							t->name = name3;
							return t;
						}
						ungetc(c,f);								//same thing again and again , if it's not matching, move 1 char back and return token
						t->type = token_assign;

						name2 = "=";
						t->name = name2;
						return t;
					//notEqual
					case '!':
						tempc = c;			//not sure what does this mean, maybe I'll delete it later
						c=fgetc(f);
						if(c == '=') {
							t->type = token_notEqual;
							name3 = "!=";
							t->name = name3;
							return t;
						}
						t->type = token_invalid;	//invalid combination !xx
						return t;
					//RelOp
					case '<':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_lessEqual;

							name3 = "<=";
							t->name = name3;
							return t;
						}
						ungetc(c,f);
						t->type = token_less;

						name2 = "<";
						t->name = name2;
						return t;
					case '>':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_greaterEqual;

							name3 = ">=";
							t->name = name3;
							return t;
						}
						ungetc(c,f);
						t->type = token_greater;

						name2 = ">";
						t->name = name2;
						return t;
					case EOF:										//Damn we made it to EOF; ggwp
						t->type = token_EOF;
						return t;
					default:
						if(isdigit(c)){								//if c is number, set state to readingNumber
							state = state_readingNumber;
							tempc = c;
						}else {										//strings and numbers are alredy handled, it must be ID or KW
							state = state_readingIdentifier;
						}
						buff[position] = c;							//buffer it
						position++;
						break;
				}
				break;
		}
	}	//while 1 ends here
}

 ///TESTING SECTION DON'T DELETE
/*
void identifyToken(Token *tempTok) {
	if(tempTok->type == token_identifier) printf("id ");
	if(tempTok->type == token_invalid) printf("invalid ");
	if(tempTok->type == token_assign) printf("= ");
	if(tempTok->type == token_EOF) printf("EOF ");
	if(tempTok->type == token_boolean) printf("boolean ");
	if(tempTok->type == token_break) printf("break ");
	if(tempTok->type == token_class) printf("class ");
	if(tempTok->type == token_continue) printf("continue ");
	if(tempTok->type == token_do) printf("do ");
	if(tempTok->type == token_double) printf("double ");
	if(tempTok->type == token_else) printf("else ");
	if(tempTok->type == token_false) printf("false ");
	if(tempTok->type == token_for) printf("for ");
	if(tempTok->type == token_if) printf("if ");
	if(tempTok->type == token_int) printf("int ");
	if(tempTok->type == token_return) printf("return ");
	if(tempTok->type == token_String) printf("String ");
	if(tempTok->type == token_static) printf("static ");
	if(tempTok->type == token_true) printf("true ");
	if(tempTok->type == token_void) printf("void ");
	if(tempTok->type == token_while) printf("while ");
	if(tempTok->type == token_equal) printf("equal ");
	if(tempTok->type == token_greater) printf("greater ");
	if(tempTok->type == token_less) printf("less ");
	if(tempTok->type == token_notEqual) printf("notEqual ");
	if(tempTok->type == token_greaterEqual) printf("greaterEqual ");
	if(tempTok->type == token_lessEqual) printf("lessEqual ");
	if(tempTok->type == token_add) printf("add ");
	if(tempTok->type == token_subtract) printf("subtract ");
	if(tempTok->type == token_multiply) printf("multiply ");
	if(tempTok->type == token_divide) printf("divide ");
	if(tempTok->type == token_dot) printf("dot ");
	if(tempTok->type == token_bracketLeftRound) printf("bracketLeftRound ");
	if(tempTok->type == token_bracketRightRound) printf("bracketRightRound ");
	if(tempTok->type == token_comma) printf("comma ");
	if(tempTok->type == token_bracketLeftCurly) printf("bracketLeftCurly ");
	if(tempTok->type == token_bracketRightCurly) printf("bracketRightCurly ");
	if(tempTok->type == token_semicolon) printf("semicolon ");
	if(tempTok->type == token_bracketLeftSquare) printf("bracketLeftSquare ");
	if(tempTok->type == token_bracketRightSquare) printf("bracketRightSquare ");
	if(tempTok->type == token_quotesSingle) printf("quotesSingle ");
	if(tempTok->type == token_quotesDouble) printf("quotesDouble ");
	if(tempTok->type == token_string) printf("string ");
	if(tempTok->type == token_intNumber) printf("intNumber ");
	if(tempTok->type == token_doubleNumber) printf("doubleNumber ");
}

int main(int argc, char *argv[]) {

	FILE *f;
	f = fopen("tests/double.java", "r");
	Token *tempTok = lookAhead(f, 0);


	while(tempTok->type != token_EOF) {
		printf("%d",tempTok->type);
		//if (tempTok->name != NULL) printf("@@@%s@@@",tempTok->name);
		identifyToken(tempTok);
		tempTok = lookAhead(f, 0);
	}
	fclose(f);
	printf("\n");
	return 1;

}
*/
