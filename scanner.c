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

//#define memalloc malloc
//#define memrealloc realloc
//#define memfreeall() ;

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
    int c, position = 0, tempc = 0, kwIndex = 0, isDouble = 0, hasE = 0, isComplex = 0, end = 0;
	State_type state = state_default;					//choose between states

	if(t->type == token_invalid){						//initialization faiulure
		printf("Invalid token\n");
		return t;
	}

	while(1) {
		c = fgetc(f);
		switch (state) {								//check if I'm not in reading number/id/string phase
			case state_readingIdentifier:
				if(isalpha(c) || isdigit(c) || (c == '.' && isComplex==0) || c == '_' || c == '$'){			//id's may contain numbers and characters or 1 dot
					if(c == '.') isComplex = 1;
					if(buff[position-1] == '.') {
						if(isalpha(c) || c == '$' || c == '_') {

						}else {
							state = state_default;						//set state to default so we'll know we are not reading id anymore
							ungetc(c,f);
							ungetc('.',f);
							position--;
							end = true;
						}
					}
					if(!end){
						buff[position] = c;
						position++;
						if(position+2 == buffSize) {
							buffSize += BUFFER_SIZE;
							buff = memrealloc(buff, buffSize);
						}
					}
				}else {											//end of allowed chars
					state = state_default;						//set state to default so we'll know we are not reading id anymore
					ungetc(c,f);								//go back 1 char
				}
				if(state == state_default) {					//reading has ended
					if(buff[position-1] == '.') {
						ungetc('.',f);
						position--;
					}
					buff[position] = '\0';						//add ending character at the end
					if((kwIndex = isKeyword(buff)) != -1) {		//if buffered word is in keyword array return kw token
						t->type = kwIndex + KEYWORD_OFFSET;		//value calculated by returned kwIndex and KEYWORD_OFFSET set in scanner.h
					}else {
						t->name = buff;
						t->type = token_identifier;				//it's not keyword so return as id
					}
					position = 0;								//reset position of buffer
					isComplex = 0;								//reset isComplex
					//printf(" [%s]",buff);	//TODO:remove
					return t;
				}
				break;
			case state_readingString:							//reading string now doesn't store ""
				if(c == EOF || c == '\n') {
					memfreeall();
					fprintf(stderr, "String is incorrect.\n");
					exit(1);
				}else {
					if(c == '\\'){
						c = fgetc(f);
						if(isdigit(c)){
							char oct[] = "zz";
							oct[0] = c;
							c = fgetc(f);
							if(isdigit(c)){
								oct[1] = c;
								c = fgetc(f);
								if(isdigit(c)){
									buff[position] = '\\';
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
									buff[position] = oct[0];
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
									buff[position] = oct[1];
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
									buff[position] = c;
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
								}else {
									memfreeall();
									fprintf(stderr, "String is incorrect.\n");
									exit(1);
								}
							}else {
								memfreeall();
								fprintf(stderr, "String is incorrect.\n");
								exit(1);
							}
						}else if(c != '\"' && c != 'n' && c != 't' && c != '\\') {
							memfreeall();
							fprintf(stderr, "String is incorrect.\n");
							exit(1);
						}else {
							buff[position] = '\\';
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}
							buff[position] = c;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}
						}
					}else if(c == '\"' && (buff[position-1] != '\\' && buff[position-2] != '\\')) {		//looking for " but only if previous char is not '\', so '\"' is not matching
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
					//printf(" [%s]",buff);	//TODO:remove
					return t;
				}
				break;
			case state_readingNumber:
				if(buff[position-1] == '.' && (c == 'e' || c=='E')){
					end = true;
					isDouble = false;
				}else if(c == '.' && (buff[position-1] == 'e' || buff[position-1]=='E')){
					end = true;
					isDouble = false;
				}else if(c == '.' && hasE == false && isDouble == false){	//if there is a dot, set number as double
					isDouble = true;
				}else if((c == 'e' || c == 'E') && hasE == false) {	//when any character is <- , it is not average number, it is DOUBLE
					hasE = true;
					isDouble = true;
				}else if(c == 'e' || c=='E' || c=='.'){				//if there is E or dot when number has already set isdouble as false
					end = true;
				}

				if(isdigit(c)){										//if char is a digit, store it into buffer
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else if(!end && isDouble && (c == 'E' || c == 'e' || c == '+' || c == '-' || c == '.')){
					buff[position] = c;								//if number is evaluated as double store double-allowed chars
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else {												//current char is not digit or it is double-allowed char when isDouble is false and
					ungetc(c,f);									//undo readings
					state = state_default;							//end reading state
					if(tempc == '+' || tempc == '-' || tempc == '.') {
						ungetc(tempc, f);							//undo readings
						position--;
					}
					if(buff[position-1] == 'e' || buff[position-1] == 'E') {
						ungetc(buff[position-1], f);							//undo readings
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
					end = false;
					//printf(" [%s]",buff);	//TODO:remove
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
						fprintf(stderr, "Lexical error\n");
						memfreeall();
						exit(1);
						break;
					case '\"':									//beggining of " had been found, set state to readingString and buffer the char
						state = state_readingString;
						break;
					//Delimiters
					case '.':
						fprintf(stderr, "Lexical error\n");
						memfreeall();
						exit(1);
						break;
						//t->type = token_dot;
						//name2 = ".";
						//t->name = name2;
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
						}else if(isalpha(c) || c == '$' || c == '_') {										//strings and numbers are alredy handled, it must be ID or KW
							state = state_readingIdentifier;
						}else {
							fprintf(stderr, "Lex error\n");		//TODO: not sure
							exit(1);
						}
						buff[position] = c;							//buffer it
						position++;
						break;
				}
				break;
		}
	}
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
   printf("\n");
}

int main(int argc, char *argv[]) {

   FILE *f;
   f = fopen("tests/lex.java", "r");
   Token *tempTok = lookAhead(f, 0);


   while(tempTok->type != token_EOF) {
	   printf("\t\t%d",tempTok->type);
	   //if (tempTok->name != NULL) printf("@@@%s@@@",tempTok->name);
	   identifyToken(tempTok);
	   tempTok = lookAhead(f, 0);
   }
   fclose(f);
   printf("\n");
   return 1;

}
*/
