#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

#define true 1
#define false 0


char *keywords[] = {"boolean", "break", "class", "continue", "do", "double",
	"else", "false", "for", "if", "int", "return", "String", "static", "true","void","while"};

int isKeyword(char *string) {
    int i;
    for (i = 0; i < 17; i++) {
        if (!strcmp(keywords[i], string))
            return i;
    }
    return -1;
}

Token *tokenInit() {
	Token *t = (Token *)malloc(sizeof(Token));
	if(t == NULL) {
		printf("tokenInit malloc error\n");
		t->type = token_invalid;
	}
	return t;
}

Token *lookAhead(FILE *f, int steps) { //TODO : Is there better way of passing FILE?
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
			if(tNext == NULL) {
				t = getToken(f);
			}else {
				t = tNext;
				tNext = NULL;
			}
			return t;
			break;
		case 1:
			//t = getToken(f);
			tNext = getToken(f);
			return tNext;
			break;
		default:
			break;
	}
}

Token *getToken(FILE *f) { 	//TODO : Is there better way of passing FILE? 	//Call lookAhead instead of getToken();

	//printf("AYA");
	Token *t = tokenInit();	//TODO Kuba-edit
	char buff[1024];
    int c, position = 0, tempc, kwIndex, isDouble;
	State_type state = state_default;
	//t = malloc(sizeof(Token));
	//tokenInit(t);
	//t->type = token_invalid;
	if(t->type == token_invalid){
		printf("Invalid token\n");
		return t;
	}


	while(1) {					//TODO ?
		c = fgetc(f);
		//printf("%c,",c);
		switch (state) {
			case state_readingIdentifier:
				if(isalpha(c) || isdigit(c)){
					buff[position] = c;
					position++;
				}else {
					state = state_default;
					ungetc(c,f);
				}
				if(state == state_default) {
					buff[position] = '\0';
					if((kwIndex = isKeyword(buff)) != -1) {
						t->type = kwIndex + KEYWORD_OFFSET;
					}else {
						t->type = token_identifier;
					}
					position = 0;
					return t;
				}
				break;
			case state_readingString:
				buff[position] = c;
				position++;
				if(c == '\"' && buff[position-2] != '\\') {
					state = state_default;
				}
				if(state == state_default) {
					buff[position] = '\0';
					t->type = token_string;
						printf("'%s'",buff);
					position = 0;
					return t;
				}
				break;
			case state_readingNumber:
				if(c == '.' || c == 'e' || c == 'E') {
					isDouble = true;
				}

				if(isdigit(c)){
					buff[position] = c;
					position++;
				}else if(isDouble && (c == 'E' || c == 'e' || c == '+' || c == '-' || c == '.')){
					buff[position] = c;
					position++;
				}else { //Osetrit double
					ungetc(c,f);
					state = state_default;
					//readingNumber = false;
				}
				if(state == state_default) {
					buff[position] = '\0';
					if(isDouble) {
						t->type = token_doubleNumber;
						printf("DABL'%s'",buff);
					}else {
						t->type = token_intNumber;
						printf("INT'%s'",buff);
					}
					position = 0;
					isDouble = false;
					return t;
				}
				break;
			case state_default:
				switch (c) {
					case ' ':
						break;
					case '\n':
						break;
					case '\t':
						break;
					//ArOp
					case '+':
						t->type = token_add;
						return t;
					case '-':
						t->type = token_subtract;
						return t;
					case '*':
						t->type = token_multiply;
						return t;
					//Division or Comments
					case '/':
						c = fgetc(f);
						if(c == '/') {
							while((c=fgetc(f)) != '\n'){
								;
							}
							break;
						}else if(c == '*') {
							//printf("K");
							tempc = ' ';
							while(((c=fgetc(f)) != '/') || (tempc != '*')) {
								tempc = c;
							}
							break;
						}else {
							ungetc(c,f);
							t->type = token_divide;
							return t;
						}
					case '\'':
						break;
					case '\"':
						state = state_readingString;
						buff[position] = c;
						position++;
						break;
					//Delimiters
					case '.':
						t->type = token_dot;
						return t;
					case '(':
						t->type = token_bracketLeftRound;
						return t;
					case ')':
						t->type = token_bracketRightRound;
						return t;
					case ',':
						t->type = token_comma;
						return t;
					case '{':
						t->type = token_bracketLeftCurly;
						return t;
					case '}':
						t->type = token_bracketRightCurly;
						return t;
					case ';':
						t->type = token_semicolon;
						return t;
					case '[':
						t->type = token_bracketLeftSquare;
						return t;
					case ']':
						t->type = token_bracketRightSquare;
						return t;
					//assign =, equal ==
					case '=':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_equal;
							return t;
						}
						ungetc(c,f);
						t->type = token_assign;
						return t;
					//notEqual
					case '!':
						tempc = c;
						c=fgetc(f);
						if(c == '=') {
							t->type = token_notEqual;
							return t;
						}
						t->type = token_invalid;	//invalid combination !xx
						return t;
					//RelOp
					case '<':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_lessEqual;
							return t;
						}
						ungetc(c,f);
						t->type = token_less;
						return t;
					case '>':
						c = fgetc(f);
						if(c == '=') {
							t->type = token_greaterEqual;
							return t;
						}
						ungetc(c,f);
						t->type = token_greater;
						return t;
					case EOF:
						t->type = token_EOF;
						return t;
					default:
						if(isdigit(c)){
							state = state_readingNumber;
						}else {
							state = state_readingIdentifier;
						}
						buff[position] = c;
						position++;
						break;
				}
				break;
		}
	}	//while 1 ends here
}


/*  ///TESTING SECTION DON'T DELETE
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
	f = fopen("test2.java", "r");
	Token *tempTok = lookAhead(f, 0);
	//Token *firstTok = lookAhead(f, 0);
//
	identifyToken(tempTok);
	Token *nextTok = lookAhead(f, 1);
	identifyToken(nextTok);
	tempTok = lookAhead(f, 0);
	identifyToken(tempTok);
	nextTok = lookAhead(f, 1);
	identifyToken(nextTok);
	tempTok = lookAhead(f, 0);
	identifyToken(tempTok);
	tempTok = lookAhead(f, 0);
	identifyToken(tempTok);
	tempTok = lookAhead(f, 0);
	identifyToken(tempTok);
	nextTok = lookAhead(f, 1);
	identifyToken(nextTok);
	tempTok = lookAhead(f, 0);
	identifyToken(tempTok);

	while(tempTok->type != token_EOF) {
		printf("%d",tempTok->type);
		identifyToken(tempTok);
		tempTok = lookAhead(f, 0);
	}
	fclose(f);
	printf("\n");
	return 1;

}
 */
