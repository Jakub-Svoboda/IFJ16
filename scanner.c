#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

#define true 1
#define false 0
#define typeOffset 4


/*

	musim udelat doubly a rozpoznavani exponentu,,

char *relationalOperators[] = {"==", "<", ">", "!=", ">=", "<="};

char arithmeticalOperators[] = {'+','-','*','/'};

char delimiters[] = {'.', '(', ')', ',', '{', '}', ';', '[', ']'};


int get__Token(FILE *f) {
	Token kekel;
    unsigned count = 0, i;
    char buff[1024];
    int c;

    while((c = fgetc(f)) != ' ' && c != EOL && c!= EOF && c!= TAB) {
        buff[count] = c;
        count++;
    }
	if(c != ' ' && c!= TAB && c!= EOF ){

		buff[count] = '\0';
		printf("%s||",buff);
	}

	if(c == EOF) {
		return 0;
	}
    return 1;
}
*/

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

Token *getToken(FILE *f) {
	
	//printf("AYA");
	Token *t = tokenInit();	//TODO Kuba-edit
	char buff[1024];
    int c, position = 0, tempc, readingIdentifier = false, kwIndex, readingString = false, readingNumber = false;
	
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
		if(readingIdentifier) {
			if(isalpha(c) || isdigit(c)){
				buff[position] = c;
				position++;
			}else {
				readingIdentifier = false;
				ungetc(c,f);
			}
			if(readingIdentifier == false) {
				buff[position] = '\0';
				if((kwIndex = isKeyword(buff)) != -1) {
					t->type = kwIndex + typeOffset;
					//printf("repete %d ",kwIndex+typeOffset);
				}else {
					printf("ID'%s'\n",buff);						//TODO delete later
					t->type = token_identifier;
				}
				position = 0;
				return t;
			}
		}else if(readingString){
			buff[position] = c;
			position++;
			if(c == '\"' && buff[position-2] != '\\') {
				readingString = false;
			}
			if(readingString == false) {
				buff[position] = '\0';
				t->type = token_string;
					printf("'%s'",buff);
				position = 0;
				return t;
				
			}
		}else if(readingNumber) {
			//buff[position] = c;
			position++;
			if(isdigit(c)) {
				buff[position] = c;
				position++;
			}else { //Osetrit double
				ungetc(c,f);
				readingNumber = false;
			}
			if(readingNumber == false) {
				buff[position] = '\0';
				t->type = token_intNumber;
					printf("'%s'",buff);
				position = 0;
				return t;
				
			}
		}else {
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
					readingString = true;
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
						readingNumber = true;
					}else {
						readingIdentifier = true;
					}
					buff[position] = c;
					position++;
					break;
			}
			
		}

	}



}

/*
int main(int argc, char *argv[]) {

	FILE *f;
	f = fopen("test2.java", "r");
	Token *tempTok = getToken(f);
	while(tempTok->type != token_EOF) {
		printf("%d",tempTok->type);
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
		tempTok = getToken(f);
	}
	fclose(f);
	printf("\n");
	return 1;
	
}*/