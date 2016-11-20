/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    scanner.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
//#include "htab.c"

#define true 1
#define false 0



int isKeyword(char *string) {							//return index value of choosen keyword in array
	char *keywords[] = {"boolean", "break", "class", "continue", "do", "double",
		"else", "false", "for", "if", "int", "return", "String", "static", "true","void","while"};
    int i;
    for (i = 0; i < 17; i++) {
        if (!strcmp(keywords[i], string))
            return i;
    }
    return -1;											//if string is not keyword return -1
}

Token *tokenInit() {									//allocate space
	Token *t = (Token *)memalloc(sizeof(Token));
	
	t->type=-1;
	t->name = NULL;
	return t;
}

Token *lookAhead(FILE *f, int steps) { 	//take a look at the next token without losing it permanently
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

Token *getToken(FILE *f) { 								//Call lookAhead instead of getToken();

	int buffSize = BUFFER_SIZE;
	char *buff = (char*) memalloc(buffSize * sizeof(char));
	char* name2 = memalloc(sizeof(char)*2);
	char* name3 = memalloc(sizeof(char)*3);

	Token *t = tokenInit();

	//tempc is buffered character, kwIndex is ord. value of keyword type, isDouble is boolean-like var
    int c, position = 0, tempc = 0, kwIndex = 0, isDouble = 0, hasE = 0, isComplex = 0;
	State_type state = state_default;					//choose between states

	if(t->type == token_invalid){						//initialization faiulure
		printf("Invalid token\n");
		return t;
	}

	while(1) {
		c = fgetc(f);
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
				}else {											//end of allowed chars
					state = state_default;						//set state to default so we'll know we are not reading id anymore
					ungetc(c,f);								//go back 1 char
				}
				if(state == state_default) {					//reading has ended
					buff[position] = '\0';						//add ending character at the end
					if((kwIndex = isKeyword(buff)) != -1) {		//if buffered word is in keyword array return kw token
						t->type = kwIndex + KEYWORD_OFFSET;		//value calculated by returned kwIndex and KEYWORD_OFFSET set in scanner.h
					}else {
						t->name = buff;
						t->type = token_identifier;				//it's not keyword so return as id
					}
					position = 0;								//reset position of buffer
					isComplex = 0;								//reset isComplex
					return t;
				}
				break;
			case state_readingString:							//reading string now doesn't store ""
				if(c == EOF) {
					memfreeall();
					fprintf(stderr, "String is incorrect.\n");
					exit(1);
				}else {
					if(c == '\"' && (buff[position-1] != '\\' && buff[position-2] != '\\')) {		//looking for " but only if previous char is not '\', so '\"' is not matching
						state = state_default;								//if found, end reading
					}else if((c == '\"' && buff[position-2] == '\\')) {		//looking for " but only if previous char is not '\', so '\"' is not matching
						state = state_default;								//if found, end reading
					}else {
						buff[position] = c;
						position++;
						if(position+2 == buffSize) {
							buffSize += BUFFER_SIZE;
							buff = memrealloc(buff, buffSize);
						}
					}
				}
				if(state == state_default) {			//same as readingIdentifier
					buff[position] = '\0';
					t->type = token_string;
					t->name = buff;
					position = 0;
					return t;
				}
				break;
			case state_readingNumber:
				if(c == '.' && hasE == false && isDouble == false){	//if there is a dot, set number as double
					isDouble = true;
				}else if((c == 'e' || c == 'E') && hasE == false) {	//when any character is <- , it is not average number, it is DOUBLE
					hasE = true;
				}else if(c == 'e' || c=='E' || c=='.'){				//if there is E or dot when number has already set isdouble as false
					isDouble = false;
				}

				if(isdigit(c)){										//if char is a digit, store it into buffer
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else if(isDouble && (c == 'E' || c == 'e' || c == '+' || c == '-' || c == '.')){
					buff[position] = c;								//if number is evaluated as double store double-allowed chars
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else {												//current char is not digit or it is double-allowed char when isDouble is false and
					ungetc(c,f);									//undo readings
					state = state_default;							//end reading state
					if(tempc == '+' || tempc == '-') {
						ungetc(tempc, f);							//undo readings
						position--;
					}
				}
				if(state == state_default) {
					buff[position] = '\0';
					if((isDouble) || (!isDouble && hasE)) {
						t->type = token_doubleNumber;
						t->name = buff;
					}else {
						t->type = token_intNumber;
						t->name = buff;
					}
					position = 0;								//reset
					isDouble = false;							//reset
					hasE = false;
					return t;
				}
				tempc = c;
				break;
			case state_default:									//this will be done at every beginning of token reading
				switch (c) {
					case ' ':									//skip chars like spaces, newlines and tabs
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
							}

							break;
						}else if(c == '*') {								//block-comment found
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
						fprintf(stderr, "No token matches '\n");
						memfreeall();
						exit(1);
						break;
					case '\"':									//beggining of " had been found, set state to readingString and buffer the char
						state = state_readingString;
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
	}
}
