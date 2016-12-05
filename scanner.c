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
#include <math.h>
//#include "htab.c"

//#define memalloc malloc
//#define memrealloc realloc
//#define memfreeall() ;

#define true 1
#define false 0

int ipowLex(int base, int exp)
{
    int num = 1;
    while (exp != 0) {
        if ((exp & 1) == 1)
            num *= base;
        exp >>= 1;
        base *= base;
    }
    return num;
}

char* intToStr(int num) {
	int count = 0, temp = num;
	while(temp != 0) {
		count++;
        temp/=10;
    }
	char *buff = (char*) memalloc((count + 1) * sizeof(char));
	for(int i = count-1; i >= 0; i--) {
		buff[i] = '0' + num%10;
		//printf("[%c]",buff[i]);  //TODO: remove
		num/=10;
	}
	buff[count] = '\0';
	//printf("%s coun,",buff);  //TODO: remove
	return buff;
}


char* octToDecLex(int oct)
{
    int dec = 0, i = 0;
    while(oct != 0) {
        dec += (oct%10) * ipowLex(8,i);
        ++i;
        oct/=10;
    }
    return intToStr(dec);
}

char* binToDecLex(int bin)
{
    int dec = 0, i = 0;
    while(bin != 0) {
        dec += (bin%10) * ipowLex(2,i);
        ++i;
        bin/=10;
    }
	return intToStr(dec);
}

char* hexadecToDecLex(char* hexadec, int isDouble, int hasDot, int hasHaxE)
{
	//printf("hexereere %s %d %d %d prislo", hexadec,isDouble,hasDot,hasHaxE); //TODO remove
    int dec = 0, i = 0, count = 0, expSign = 0, expNum = 0, quiet;
	double decDouble = 0, mantissa = 0;
	char* temp = hexadec;
	if(isDouble == 0) {
		//printf("neni double\n" ); //TODO remove
		while(*temp != '\0'){
			;
			quiet = *temp++;
		}
		quiet = quiet;
		quiet = *temp--;
	    while(temp != hexadec) {
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			//printf("[[%c %d]]",*temp, i);  //TODO remove
			dec += i * ipowLex(16,count);
	        ++count;
			temp--;
	    }
	}else if(isDouble == 1 && hasDot == 1 && hasHaxE != 1) {
		memfreeall();
		fprintf(stderr, "Lexical error, invalid hexadecimal number.\n");
		exit(1);
	/*	printf("je double s teckou\n" );
		while(*temp != '.'){
			;
			*temp++;
		}
		char* dot = temp;
		*temp--;
		while(temp != hexadec) {
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			printf("[[%c %d]]",*temp, i);
			decDouble += i * ipowLex(16,count);
			++count;
			temp--;

		}
		//calc .part
		count = 0; 		//reset counter
		*dot++; 		//skip the dot
		while(*dot != '\0'){
			printf("%c", *dot);
			if(*dot == 'A') {
				i = 10;
			}else if(*dot == 'B'){
				i = 11;
			}else if(*dot == 'C'){
				i = 12;
			}else if(*dot == 'D'){
				i = 13;
			}else if(*dot == 'E'){
				i = 14;
			}else if(*dot == 'F'){
				i = 15;
			}else {
				i = (*dot) - '0';
			}
			printf("[[%c %d]]",*dot, i);
			++count;
			mantissa += i * (1.0/(double)ipowLex(16,count));
			dot++;
		}
  		decDouble += mantissa;
		printf("cislo je %f \n", decDouble);
	}*/}else if(isDouble == 1 && hasDot == 1 && hasHaxE == 1) {  //hexadec with dot and exp
		//printf("je double s teckou a exp {%s} \n",hexadec ); //TODO remove
		while(*temp != '.'){
			;
			quiet = *temp++;
		}
		char* dot = temp;
		quiet = *temp--;
		while(temp != hexadec) {
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			//printf("[[%c %d]]",*temp, i); //TODO remove
			decDouble += i * ipowLex(16,count);
			++count;
			temp--;
		}
		//calc .part
		count = 0; 		//reset counter
		dot++; 			//skip the dot
		while(*dot != 'p' && *dot != 'P'){
			//printf("%c", *dot); //TODO remove
			if(*dot == 'A') {
				i = 10;
			}else if(*dot == 'B'){
				i = 11;
			}else if(*dot == 'C'){
				i = 12;
			}else if(*dot == 'D'){
				i = 13;
			}else if(*dot == 'E'){
				i = 14;
			}else if(*dot == 'F'){
				i = 15;
			}else {
				i = (*dot) - '0';
			}
			printf("[[%c %d]]",*dot, i);
			++count;
			mantissa += i * (1.0/(double)ipowLex(16,count));
			dot++;
		}
		//printf("[%c hleda]",*dot); //TODO remove
		dot++;
		if(*dot == '+') {
			expSign = 1;
		}else if(*dot == '-') {
			expSign = -1;
		}
		dot++;
		while(*dot != '\0') {
	        expNum += (*dot) - '0';
			dot++;
			if(*dot != '\0') {
				expNum *= 10;
			}
			//printf("{%d} expNum", expNum); //TODO remove
	    }

	//	printf("num %f . %f p %d exp %d", decDouble,mantissa, expSign, expNum); //TODO remove
		decDouble = decDouble + mantissa;
		if(expSign == 1) {
			decDouble *= pow(2, expNum);
		}else if(expSign == -1) {
			decDouble *= pow(2, -expNum);
		}
	//	printf("dec double je %f\n",decDouble ); //TODO remove

	}else if(isDouble == 1 && hasDot != 1 && hasHaxE == 1) {  //hexadec with dot and exp
	//	printf("je double bez tecky a exp {%s} \n",hexadec ); //TODO remove
		while(*temp != 'p' && *temp != 'P'){
			;
			quiet = *temp++;
		}
		char* dot = temp;
		quiet = *temp--;
		while(temp != hexadec) {
			if(*temp == 'A') {
				i = 10;
			}else if(*temp == 'B'){
				i = 11;
			}else if(*temp == 'C'){
				i = 12;
			}else if(*temp == 'D'){
				i = 13;
			}else if(*temp == 'E'){
				i = 14;
			}else if(*temp == 'F'){
				i = 15;
			}else {
				i = (*temp) - '0';
			}
			printf("[[%c %d]]",*temp, i);
			dec += i * ipowLex(16,count);
			++count;
			temp--;
		}
		//calc .part
		count = 0; 		//reset counter
		dot++; 		//skip the dot
		if(*dot == '+') {
			expSign = 1;
		}else if(*dot == '-') {
			expSign = -1;
		}
		dot++;
		while(*dot != '\0') {
	        expNum += (*dot) - '0';
			dot++;
			if(*dot != '\0') {
				expNum *= 10;
			}
		//	printf("{%d} expNum", expNum); //TODO remove
	    }

	//	printf("num %d p %d exp %d", dec, expSign, expNum); //TODO remove
		decDouble = (double)dec;
		if(expSign == 1) {
			decDouble *= pow(2, expNum);
		}else if(expSign == -1) {
			decDouble *= pow(2, -expNum);
		}
	//	printf(" Doublas je %f\n", decDouble);  //TODO remove
	}
	char* tempDouble = memalloc(2048 * sizeof(char));
	char* buffDouble;
	int len = 0;
	if(decDouble != 0) {
		len = sprintf(tempDouble,"%lf",decDouble);
		buffDouble = memalloc(len * sizeof(char));
		sprintf(buffDouble,"%d",dec);
	}else {
		len = sprintf(tempDouble,"%d",dec);
		buffDouble = memalloc(len * sizeof(char));
		sprintf(buffDouble,"%d",dec);
	}
	//printf("hexedec vraci {%s}",buffDouble); //TODO remove
	return buffDouble;
	//return intToStr(dec);
}


int isKeyword(char *string) {							//return index value of choosen keyword in array
	char *keywords[] = {"boolean", "break", "class", "continue", "do", "double",
		"else", "false", "for", "if", "int", "return", "String", "static", "true","void","while"};
    int i;
    for (i = 0; i < 17; i++) {
        if (!strcmp(keywords[i], string)){
            return i;
		}
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
	int compSize = BUFFER_SIZE;
	char *buff = (char*) memalloc(buffSize * sizeof(char));
	char *complex = (char*) memalloc(buffSize * sizeof(char));

	char* name2 = memalloc(sizeof(char)*2);
	char* name3 = memalloc(sizeof(char)*3);

	Token *t = tokenInit();

	//tempc is buffered character, kwIndex is ord. value of keyword type, isDouble is boolean-like var
    int c, position = 0, tempc = 0, kwIndex = 0, isDouble = 0, hasE = 0, isComplex = 0, end = 0, complexPos = 0;
	int binInt = 0, tempest = 0, tempestc = 0, octInt = 0, hexadecInt = 0, hasDot = 0, hasHaxE = 0;
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
					if(c == '.') {
						isComplex = 1;
						if((kwIndex = isKeyword(buff)) != -1) {		//if buffered word is in keyword array return kw token
							memfreeall();
							fprintf(stderr, "Lexical error.\n");
							exit(1);
						}
					}
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
						if(isComplex == 1 && c != '.') {
							complex[complexPos] = c;
							complexPos++;
							if((kwIndex = isKeyword(complex)) != -1) {		//if buffered word is in keyword array return kw token
								memfreeall();
								fprintf(stderr, "Lexical error.\n");
								exit(1);
							}
							if(complexPos+2 == buffSize) {
								compSize += BUFFER_SIZE;
								complex = memrealloc(complex, compSize);
							}
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
					complexPos = 0;
					//printf(" [%s]",buff);	//TODO:remove
					return t;
				}
				break;
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
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
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
			case state_readingNumber:
				if(c == '_') {
					;//Ve všech soustavách lze použít
					//speciální symbol _ (podtržítko), který slouží jako oddělovač skupin číslic v čísle pro
					//zajištění lepší čitelnosti. Více viz popis číselných literálů v normě 2 jazyka Java.
					c = fgetc(f);
					if(c == '_') {

						ungetc(c, f);
					}

				}
				if(tempc == '0' && position == 1) {
					//tempest = fgetc(f);
					//printf("%cfdsf \n", tempest);
					if(c == 'b') {
						if((tempest = fgetc(f)) == '0' || tempest == '1') {
						/*	buff[position] = c;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}*/
							buff[position] = tempest;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}
							while((c = fgetc(f)) == '0' || c == '1' || c == '_') {
								binInt = 1;
								if(c == '_') {
									tempest = fgetc(f);
									if(tempest == '_') {
										break;
									}
									ungetc(tempest, f);
								}else {
									buff[position] = c;
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
								}
							}
						}else {
							ungetc(c,f);
							ungetc(tempest,f);
						}
					}else if(c == 'x') {
						if(((tempest = fgetc(f)) >= '0' && tempest <= '9') || tempest == '_' || (tempest >= 'A' && tempest <= 'F')) {

						/*	buff[position] = c;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}*/
							hexadecInt = 1;
							buff[position] = tempest;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}

							while(((c = fgetc(f)) >= '0' && c <= '9') || c == '_' || (c >= 'A' && c <= 'F')) {

								if(c == '_') {
									tempest = fgetc(f);
									if(tempest == '_') {
										break;
									}
									ungetc(tempest, f);
								}else {
									buff[position] = c;
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
								}
							}
							tempest = c;
							if(c == '.') {
								printf("jdu do tecky\n");
								hasDot = 1;
								c = fgetc(f);
								if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || c == '_'){
									buff[position] = '.';
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
									if(c == '_') {
										tempest = fgetc(f);
										if(tempest == '_') {
											break;
										}
										ungetc(tempest, f);
									}else {
										buff[position] = c;
										position++;
										if(position+2 == buffSize) {
											buffSize += BUFFER_SIZE;
											buff = memrealloc(buff, buffSize);
										}
									}
									isDouble = 1;


									//rest
									while(((c = fgetc(f)) >= '0' && c <= '9') || c == '_' || (c >= 'A' && c <= 'F')) {
										hexadecInt = 1;
										if(c == '_') {
											tempest = fgetc(f);
											if(tempest == '_') {
												break;
											}
											ungetc(tempest, f);
										}else {
											buff[position] = c;
											position++;
											if(position+2 == buffSize) {
												buffSize += BUFFER_SIZE;
												buff = memrealloc(buff, buffSize);
											}
										}
									}
									if(c == 'p' || c == 'P') {
										tempestc = c;
										if((tempest = fgetc(f)) == '+' || tempest == '-') { //0xFF.FFp-
											buff[position] = tempestc;
											position++;
											if(position+2 == buffSize) {
												buffSize += BUFFER_SIZE;
												buff = memrealloc(buff, buffSize);
											}
											buff[position] = tempest;
											position++;
											if(position+2 == buffSize) {
												buffSize += BUFFER_SIZE;
												buff = memrealloc(buff, buffSize);
											}
											hasHaxE = 1;

											while(((c = fgetc(f)) >= '0' && c <= '9') || c == '_') {
												if(c == '_' && isdigit(buff[position-1])) {
													tempest = fgetc(f);
													if(tempest == '_') {
														break;
													}
													ungetc(tempest, f);
												}else if(c == '_' && !isdigit(buff[position-1])){
													break;
												}else {
													buff[position] = c;
													position++;
													if(position+2 == buffSize) {
														buffSize += BUFFER_SIZE;
														buff = memrealloc(buff, buffSize);
													}
												}
											}
										}else {
											ungetc(tempestc, f);
											ungetc(tempest, f);
										}
									}else {
										ungetc(c, f);
									}
								}else {
									ungetc('.',f);
									ungetc(c,f);
								}
							}else if(c == 'p' || c == 'P'){	//there is no decimal point
								if(c == 'p' || c == 'P') {
									tempestc = c;
									if((tempest = fgetc(f)) == '+' || tempest == '-') { //0xFF.FFp-
										buff[position] = tempestc;
										position++;
										if(position+2 == buffSize) {
											buffSize += BUFFER_SIZE;
											buff = memrealloc(buff, buffSize);
										}
										buff[position] = tempest;
										position++;
										if(position+2 == buffSize) {
											buffSize += BUFFER_SIZE;
											buff = memrealloc(buff, buffSize);
										}
										hasHaxE = 1;
										while(((c = fgetc(f)) >= '0' && c <= '9') || c == '_') {
											if(c == '_' && isdigit(buff[position-1])) {
												tempest = fgetc(f);
												if(tempest == '_') {
													break;
												}
												ungetc(tempest, f);
											}else if(c == '_' && !isdigit(buff[position-1])){
												break;
											}else {
												isDouble = 1;
												buff[position] = c;
												position++;
												if(position+2 == buffSize) {
													buffSize += BUFFER_SIZE;
													buff = memrealloc(buff, buffSize);
												}
											}
										}
									}else {
										ungetc(tempestc, f);
										ungetc(tempest, f);
									}
								}else {
									ungetc(c, f);
								}
							}else {		//next char is not p or P
								//ungetc(c,f);		//I commented this
								//ungetc(tempest,f);
							}
						}else {
							ungetc(c,f);
							ungetc(tempest,f);
						}

					}else if(c >= '0' && c <= '7') {

							buff[position] = c;
							position++;
							if(position+2 == buffSize) {
								buffSize += BUFFER_SIZE;
								buff = memrealloc(buff, buffSize);
							}
							while(((c = fgetc(f)) >= '0' && c <= '7') || c == '_') {
								octInt = 1;
								if(c == '_') {
									tempest = fgetc(f);
									if(tempest == '_') {
										break;
									}
									ungetc(tempest, f);
								}else {
									buff[position] = c;
									position++;
									if(position+2 == buffSize) {
										buffSize += BUFFER_SIZE;
										buff = memrealloc(buff, buffSize);
									}
								}
							}
					}
				}

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

			/*	if((binInt == 1 && (c == '0' || c == '1')) || (c == 'b' && position == 1) && octInt != 1 && hexadecInt != 1) {
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else */if(isdigit(c) && binInt != 1 && octInt != 1 && hexadecInt != 1){										//if char is a digit, store it into buffer
					buff[position] = c;
					position++;
					if(position+2 == buffSize) {
						buffSize += BUFFER_SIZE;
						buff = memrealloc(buff, buffSize);
					}
				}else if(!end && isDouble && (c == 'E' || c == 'e' || c == '+' || c == '-' || c == '.')  && binInt != 1 && octInt != 1 && hexadecInt != 1){
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
					if(buff[position-1] == 'e' || (buff[position-1] == 'E' && hexadecInt == 0)) {
						ungetc(buff[position-1], f);							//undo readings
						position--;
					}
				}
				if(state == state_default) {
					buff[position] = '\0';
					if(binInt == 1) {
						buff = binToDecLex(atoi(buff));
					}else if(octInt == 1) {
						buff = octToDecLex(atoi(buff));
					}else if(hexadecInt == 1) {
					//	printf(" posilam %s haha\n", buff); //TODO: remove
						buff = hexadecToDecLex(buff, isDouble, hasDot, hasHaxE);
					}
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
					hasHaxE = 0;
					hasDot = 0;
					end = false;
					binInt = 0;
					octInt = 0;
					hexadecInt = 0;
					//printf(" buff[%s]",buff);	//TODO:remove
					return t;
				}
				tempc = c;
				break;
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
/*000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000*/
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
   f = fopen("tests/correct/printSquare.java", "r");
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
