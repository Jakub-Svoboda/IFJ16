/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    syntax.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "syntax.h"

	extern resourceStruct* resources;
/* Debug function. Converts token type to string and prints it out. */
void printType(Token* tokenPtr){
	switch (tokenPtr -> type){
		case		token_add: 				fprintf(stderr,"+ \n");		break;
		case		token_subtract:	 		fprintf(stderr,"- \n");		break;
		case		token_multiply:	 		fprintf(stderr,"* \n");		break;
		case		token_divide: 			fprintf(stderr,"/ \n");		break;
		case		token_less:				fprintf(stderr,"> \n");		break;
		case		token_greater: 			fprintf(stderr,"< \n");		break;
		case		token_equal: 			fprintf(stderr,"== \n");		break;
		case		token_lessEqual: 		fprintf(stderr,"<= \n");		break;
		case		token_greaterEqual: 	fprintf(stderr,">= \n");		break;
		case		token_notEqual: 		fprintf(stderr,"!= \n");		break;
		case		token_bracketLeftRound: fprintf(stderr,"( \n");		break;
		case		token_bracketRightRound:fprintf(stderr,") \n");		break;
		case		token_identifier: 		fprintf(stderr,"ID \n");		break;
		case		token_dollar: 			fprintf(stderr,"$ \n");		break;
		case		token_expression: 		fprintf(stderr,"E \n");		break;
		case		token_invalid: 			fprintf(stderr,"invalid \n");		break;
		case		token_assign: 			fprintf(stderr,"= \n");		break;
		case		token_EOF: 				fprintf(stderr,"EOF \n");		break;
		case		token_boolean: 			fprintf(stderr,"Bool \n");		break;
		case		token_break: 			fprintf(stderr,"Break \n");break;
		case		token_class: 			fprintf(stderr,"class \n");break;
		case		token_continue: 		fprintf(stderr,"continue \n");break;
		case		token_do: 				fprintf(stderr,"do \n");break;
		case		token_double: 			fprintf(stderr,"double \n");break;
		case		token_else: 			fprintf(stderr,"else \n");break;
		case		token_false: 			fprintf(stderr,"false \n");break;
		case		token_for: 				fprintf(stderr,"for \n");break;
		case		token_if: 				fprintf(stderr,"if \n");break;
		case		token_int: 				fprintf(stderr,"int \n");break;
		case		token_return: 			fprintf(stderr,"return \n");break;
		case		token_String: 			fprintf(stderr,"String \n");break;
		case		token_static: 			fprintf(stderr,"static \n");break;
		case		token_true: 			fprintf(stderr,"true \n");break;
		case		token_void: 			fprintf(stderr,"void \n");break;
		case		token_while: 			fprintf(stderr,"while \n");break;
		case		token_dot: 				fprintf(stderr,". \n");break;
		case		token_comma: 			fprintf(stderr,", \n");break;
		case		token_bracketLeftCurly: fprintf(stderr,"{ \n");break;
		case		token_bracketRightCurly:fprintf(stderr,"} \n");break;
		case		token_semicolon: 		fprintf(stderr,"; \n");break;
		case		token_bracketLeftSquare:fprintf(stderr,"[ \n");break;
		case		token_bracketRightSquare: fprintf(stderr,"] \n");break;
		case		token_quotesSingle: 	fprintf(stderr,"\' \n");break;
		case		token_quotesDouble:		fprintf(stderr,"\" \n");break;
		case		token_string: 			fprintf(stderr,"string \n");break;
		case		token_intNumber: 		fprintf(stderr,"intNumber \n");break;
		case		token_doubleNumber:		fprintf(stderr,"doubleNumber \n");break;
		case		token_leftHandle: 		fprintf(stderr,"<handle \n");break;
		case		token_rightHandle: 		fprintf(stderr,">handle \n");break;

	}
}

/* Main calls this function when syntax is needed to be checked. Alocates memory for token to be copied to and then calls checkSyntax. */
int runSyntaxAnalysis (FILE *f, tListOfInstr * list) {
	tInstr I;				//An instruction to be inserted to the end of instruction list
	Token* lastToken = memalloc(sizeof(Token));			//allocate memory for last token
	Token* tokenPtr = memalloc(sizeof(Token));			//allocate memory for current token
	resources->classPtr= memalloc(sizeof(Token));		//allocate memory for pointer to current class token
	resources->funcPtr= memalloc(sizeof(Token));		//allocate memory for pointer to current function token
	generateInstruction(I,I_PROGRAM, "", "", "",list);	//beginning of instruction list instruction is generated
	int result = syntaxCheck(CLASS_BLOCK,f,tokenPtr,lastToken,list);		//syntax check begins
	return result;
}

/* Function that recognizes a builtin function by its ID and then in checks correct syntax, arguments and generates apropriate instructions
Returns 0 if the called function is not builtin and 1 if it is.*/
int builtin(FILE *f, Token * tokenPtr, tListOfInstr * list, char *var){						//return 1 if the called function is builtin function, returns 0 if not
	tInstr I;
	if(strcmp(tokenPtr->name,"ifj16.readInt") == 0){
		getModifiedToken(f,tokenPtr);	
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}				//Read next token, which must be ;
		generateInstruction(I,I_READ_INT, var, "", "",list);				//Tokens were OK, generate the propriate instruction
		return 1;
		
	}else if (strcmp(tokenPtr->name,"ifj16.readString") == 0){
		getModifiedToken(f,tokenPtr);	
		if(tokenPtr->type != token_bracketLeftRound) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}			//Read next token, which must be ;
		generateInstruction(I,I_READ_STRING, var, "", "",list);				//Tokens were OK, generate the propriate instruction	
		return 1;
		
	}else if (strcmp(tokenPtr->name,"ifj16.readDouble") == 0){
		getModifiedToken(f,tokenPtr);	
		if(tokenPtr->type != token_bracketLeftRound) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon) {memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}				//Read next token, which must be ;
		generateInstruction(I,I_READ_DOUBLE, var, "", "",list);				//Tokens were OK, generate the propriate instruction
		return 1;
		
	}else if (strcmp(tokenPtr->name,"ifj16.print") == 0){	
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		while(tokenPtr->type != token_bracketRightRound){						//Read next token, which must be )
			if(tokenPtr->type == 12){			
				generateInstruction(I,I_PRINT, tokenPtr->name, "1", "",list);
				getModifiedToken(f,tokenPtr);
				if(tokenPtr->type==token_add){getModifiedToken(f,tokenPtr);}	
			}else if (tokenPtr->type == 45){ 									//if int
				generateInstruction(I,I_PRINT, tokenPtr->name, "0", "",list);
				getModifiedToken(f,tokenPtr);
				if(tokenPtr->type==token_add){getModifiedToken(f,tokenPtr);}	//Read next token, which must be +
			}else if (tokenPtr->type == 46){ 									//if double
				generateInstruction(I,I_PRINT, tokenPtr->name,"0", "",list);
				getModifiedToken(f,tokenPtr);
				if(tokenPtr->type==token_add){getModifiedToken(f,tokenPtr);}	//Read next token, which must be +
			}else if (tokenPtr->type == 44){ 									//if string
				generateInstruction(I,I_PRINT, tokenPtr->name, "0", "",list);	
				getModifiedToken(f,tokenPtr);
				if(tokenPtr->type==token_add){getModifiedToken(f,tokenPtr);}	//Read next token, which must be +			
			}else{
				fprintf(stderr,"Syntax error. Unexpected argument ifj16.print\n");
				memfreeall();
				exit(2);
			}	
		}
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be ;
		return 1;

	}else if (strcmp(tokenPtr->name,"ifj16.length") == 0){
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_LENGTH, tokenPtr->name, "0", var,list);
			getModifiedToken(f,tokenPtr);
		}
		else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_LENGTH, tokenPtr->name, "1", var,list);
			getModifiedToken(f,tokenPtr);
		}
		else{
			fprintf(stderr,"Syntax error. Unexpected argument ifj16.length\n");
			memfreeall();
			exit(2);
		}
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be ;
		return 1;

	}else if (strcmp(tokenPtr->name,"ifj16.substr") == 0){	
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_SUBSTR1, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_SUBSTR1, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument substr\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_comma){memfreeall();fprintf(stderr,", expected\n"); exit(2);} 	//Read next token, which must be ","
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_intNumber){
			generateInstruction(I,I_SUBSTR2, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_SUBSTR2, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument substr\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_comma){memfreeall(); fprintf(stderr,", expected\n"); exit(2);} 	//Read next token, which must be ","
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_intNumber){
			generateInstruction(I,I_SUBSTR3, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_SUBSTR3, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument substr\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr,") expected\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr,"; expected\n"); exit(2);}		//Read next token, which must be ;
		generateInstruction(I,I_SUBSTR4, var, "", "",list);
		return 1;

	}else if (strcmp(tokenPtr->name,"ifj16.compare") == 0){
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_COMPARE1, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_COMPARE1, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument compare\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_comma){memfreeall();fprintf(stderr,", expected\n"); exit(2);} 	//Read next token, which must be ","
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_COMPARE2, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_COMPARE2, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument compare\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr,") expected\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr,"; expected\n"); exit(2);}		//Read next token, which must be ;
		generateInstruction(I,I_COMPARE3, var, "", "",list);
		
		return 1;
	}else if (strcmp(tokenPtr->name,"ifj16.find") == 0){
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_FIND1, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_FIND1, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument find\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_comma){memfreeall();fprintf(stderr,", expected\n"); exit(2);} 	//Read next token, which must be ","
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_FIND2, tokenPtr->name, "0", "",list);
		}else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_FIND2, tokenPtr->name, "1", "",list);
		}else{memfreeall(); fprintf(stderr, "Invalid argument find\n"); exit(2);}

		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr,") expected\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr,"; expected\n"); exit(2);}		//Read next token, which must be ;
		generateInstruction(I,I_FIND3, var, "", "",list);
		return 1;
	}else if (strcmp(tokenPtr->name,"ifj16.sort") == 0){
			getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_bracketLeftRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);} 	//Read next token, which must be (				CO MAM TED DELAT????? :'((((
			getModifiedToken(f,tokenPtr);
		if(tokenPtr->type==token_string){
			generateInstruction(I,I_SORT, tokenPtr->name, "0", var,list);
			getModifiedToken(f,tokenPtr);
		}
		else if(tokenPtr->type==token_identifier){
			generateInstruction(I,I_SORT, tokenPtr->name, "1", var,list);
			getModifiedToken(f,tokenPtr);
		}
		else{
			fprintf(stderr,"Syntax error. Unexpected argument ifj16.sort\n");
			memfreeall();
			exit(2);
		}
		if(tokenPtr->type != token_bracketRightRound){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be )
		getModifiedToken(f,tokenPtr);
		if(tokenPtr->type != token_semicolon){memfreeall(); fprintf(stderr, "syntax error\n"); exit(2);}		//Read next token, which must be ;
		return 1;	
	}else{		
		return 0;			//not a builtin function, returning 0
	}
}

/* A function that generates apropriate instruction and places it at the end of instruction list.*/
void generateInstruction(tInstr I, int instType, char addr1[2047], char addr2[2047], char addr3[2047],tListOfInstr* list){	//Adds new instruction to list of instructions
	I.instType = instType;		
	strcpy(I.addr1,addr1);
	strcpy(I.addr2,addr2);
	strcpy(I.addr3,addr3);
	listInsertLast(list, I);
}

//returns 1 if token_identifier is ID of a function
int isItFunction (FILE *f, Token* tokenPtr){
	Token * tmpPtr= lookAhead(f,1);
	if(tokenPtr->type == 12 && tmpPtr->type == 10){ //10 = bracketLeftRound, 12 = identifier
		return 1;
	}
	return 0;
}

/* Makes a copy of token into allocated memory, where all the pointer point. 
The recursive calling of syntaxCheck function would otherwise leave
the pointers from previous iteration pointing to outdate token.*/
void getModifiedToken(FILE *f,Token* tokenPtr){
	//Token * tmpPtr= getToken(f);
	Token * tmpPtr= lookAhead(f,0);
	memcpy(tokenPtr,tmpPtr,sizeof(Token));
}
/* The main function that controls syntax and generates instructions. This function is called recursively and goes to appropriate case state by
its first argument. The states copy the design of LL grammar. In each state the syntax is checked and if a mistake is found, the program ends.*/
int syntaxCheck (int state, FILE *f,Token* tokenPtr,Token* lastToken, tListOfInstr * list){
	static int breakable = 0;	//counter for dept
	int lastBreakable=0;
	static char breakJump[2047],continueJump[2047];	//Labes where break and continue should jump
	char lastBreakJump[2047], lastContinueJump[2047];
	tInstr I;
	int result=1;	
	static int counter=0; // coutner for loop labels
	static int argCount=0;		//counter for arguments
	char buf[2047];				//buffers for adresses
	char buf2[2047];
	char buf3[2047];
	
	switch (state){

//***************classBlock*******************//
		case CLASS_BLOCK:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if(tokenPtr->type != token_EOF){
					if ((result=syntaxCheck( CLASS, f, tokenPtr, lastToken, list))		!= 0) {goto EXIT;}
					if ((result=syntaxCheck( CLASS_BLOCK, f, tokenPtr, lastToken, list))		!= 0) {goto EXIT;}
			}else{
			//	fprintf(stderr, "\nSyntax OK\n");
			}
			return 0;
			break;

//******************class*******************//
		case CLASS:
			if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list))					!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
			memcpy(resources->classPtr,tokenPtr,sizeof(Token));	
			generateInstruction(I,I_CLASS,tokenPtr->name,"","",list);
			if ((result=syntaxCheck( LEFT_CURLY_BRACKET, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n{\n");goto EXIT;}
			if ((result=syntaxCheck( CLASS_BODY, f, tokenPtr, lastToken, list))			!= 0) {fprintf(stderr,"\nCB\n");goto EXIT;}
			if ((result=syntaxCheck( RIGHT_CURLY_BRACKET_CURRENT, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n}\n");goto EXIT;}

			return 0;
			break;

//******************ID*******************//
		case ID:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_identifier) {
				return 0;
			}else{
				//fprintf(stderr,"%d\n",tokenPtr->type)			;
				return -1;
			}
			break;

//******************ID_CURRENT*******************//
		case ID_CURRENT:
			if (tokenPtr->type== token_identifier) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************LEFT_CURLY_BRACKET*******************//
		case LEFT_CURLY_BRACKET:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketLeftCurly) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************Right_CURLY_BRACKET*******************//
		case RIGHT_CURLY_BRACKET:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketRightCurly) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************RIGHT_CURLY_BRACKET_CURRENT*******************//
		case RIGHT_CURLY_BRACKET_CURRENT:
			if (tokenPtr->type == token_bracketRightCurly) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************ELSE*******************//
		case ELSE:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type == token_else) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************CLASS_BODY*******************//
		case CLASS_BODY:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr ->type){
				case token_bracketRightCurly:			//If class body ends.
					return 0;
					break;
				case token_static:						//static variable of function declaration
					if ((result=syntaxCheck( TYPE, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\ntype\n");goto EXIT;}
					if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nid\n");goto EXIT;}
					sprintf(buf, "%s",tokenPtr->name);
					memcpy(resources->funcPtr,tokenPtr,sizeof(Token));		
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if (tokenPtr -> type == token_assign){			//static type id =
						sprintf(buf3, "#PRE%d",counter);
						counter++;
						generateInstruction(I,I_GLOBAL_PRE, buf3, "", "",list);
						generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
						char* buffer=runPrecedenceAnalysis(f,tokenPtr,1,list);	
						generateInstruction(I,I_MOV, buf,buffer,"", list);	
						sprintf(buf3, "#POST%d",counter-1);
						generateInstruction(I,I_GLOBAL_POST, buf3, "", "",list);
						if(tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						//printType(tokenPtr);
						break;
					}else{											//function declaration
						if (tokenPtr -> type == token_bracketLeftRound){
							generateInstruction(I,I_LABEL, concat(buf,resources->classPtr->name), "", "",list);
							if ((result=syntaxCheck( FUNCTION_DECLARE, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
							break;
						}
					}
					if (tokenPtr -> type == token_semicolon){			//semicolon after variable declaration
						break;
					}
					return result;
					break;
				default:
					return -1;
			}
			if ((result=syntaxCheck( CLASS_BODY, f, tokenPtr, lastToken, list))			!= 0) {fprintf(stderr,"\nCBC\n");goto EXIT;}
			return result;
			break;



//******************FUNCTION_DECLARE*******************//
		case FUNCTION_DECLARE:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			thtabItem *Item=htabSearch(resources->functionTable, concat(resources->funcPtr->name,resources->classPtr->name));				//find function in function table	
			if(tokenPtr -> type == token_bracketRightRound){
				if ((result=syntaxCheck( FN_BODY_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFBB\n");goto EXIT;}
			}else{
				if ((result=syntaxCheck( TYPE_CURRENT, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nTYPE\n");goto EXIT;}
					Item->arguments[argCount]=memalloc(sizeof(argStruct));																	//alloc memory for current argument
					Item->arguments[argCount]->argumentType=tokenPtr->type;																	//store type of current argument
				if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list))		!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
					Item->arguments[argCount]->argumentName=memalloc(sizeof(char)*(strlen(tokenPtr->name)+1));
					strcpy(Item->arguments[argCount]->argumentName,tokenPtr->name);	
					argCount++;						//increment position in array
					Item->argumentNumber++;
				getModifiedToken(f,tokenPtr);
				//printType(tokenPtr);
				if(tokenPtr -> type == token_comma){
					if ((result=syntaxCheck( FUNCTION_DECLARE, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFD\n");goto EXIT;}
				}else{
					if(tokenPtr -> type == token_bracketRightRound){
						argCount=0;																											//set position to zero(get ready for next function)
						if ((result=syntaxCheck( FN_BODY_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFBB\n");goto EXIT;}
					}
				}
			}
			return result;
			break;

//******************TYPE*******************//
		case TYPE:
			getModifiedToken(f,tokenPtr);					//function can return int, double, String or void
			//printType(tokenPtr);
			if (tokenPtr->type == token_int) 	{return 0;}
			if (tokenPtr->type == token_double) {return 0;}
			if (tokenPtr->type == token_String) {return 0;}
			if (tokenPtr->type == token_void) 	{return 0;}
			return -1;
			break;

//******************TYPE_CURRENT*******************//
		case TYPE_CURRENT:									//Checks currently loaded token for type
			if (tokenPtr->type == token_int) 	{return 0;}
			if (tokenPtr->type == token_double) {return 0;}
			if (tokenPtr->type == token_String) {return 0;}
			if (tokenPtr->type == token_void) 	{return 0;}


			return -1;
			break;

//******************FN_BODY_BEGIN*******************//
		case FN_BODY_BEGIN:
			if ((result=syntaxCheck( LEFT_CURLY_BRACKET, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nLCB\n");goto EXIT;}
			if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFB\n");goto EXIT;}

			return result;
			break;

//******************FN_BODY*******************//
		case FN_BODY:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch(tokenPtr -> type){
				case token_bracketRightCurly:							//end of function body
					generateInstruction(I,I_FN_END, "", "", "",list);
					if(strcmp(resources->classPtr->name,"Main")==0 && strcmp(resources->funcPtr->name,"run")==0 ){
						generateInstruction(I,I_STOP, "", "", "",list);
					}
					return 0;
					break;
				case token_identifier:								//id
					lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));	
					Token *evenLasterToken=memalloc(sizeof(Token));			//save pointer.
					evenLasterToken=memcpy(evenLasterToken,tokenPtr,sizeof(Token));	
					if(builtin(f,tokenPtr,list, "") == 1){								//check for builtin function call
						if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
						return result;
					}
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr -> type == token_bracketLeftRound){					// id(
						sprintf(buf, "%s",lastToken->name);
						resources->callPtr = lastToken;
						if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
						generateInstruction(I,I_FN_CALL, buf,"","", list);
						if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
					}else if (tokenPtr -> type == token_assign){					//id=
						sprintf(buf, "%s",lastToken->name);
						getModifiedToken(f,tokenPtr);
						if(builtin(f,tokenPtr,list,buf) == 1){						//check for builtin function call		
							//printType(tokenPtr);				
							if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
							return result;
						}
						//printType(tokenPtr);
						if (isItFunction(f,tokenPtr) == 0){							//id=EXPRESSION;
							sprintf(buf, "%s",lastToken->name);
							generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
							char* buffer=runPrecedenceAnalysis(f,tokenPtr,0,list);
							generateInstruction(I,I_MOV, buf,buffer,"", list);
							if (tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						}else{														// id = functionid
							lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));
							getModifiedToken(f,tokenPtr);
							resources->callPtr = lastToken;
							//printType(tokenPtr);
							if(tokenPtr -> type != token_bracketLeftRound){goto EXIT;} // id = functionid(
							if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
							sprintf(buf, "%s",lastToken->name);
							generateInstruction(I,I_FN_CALL, buf,"","", list);
							sprintf(buf, "%s",evenLasterToken->name);
							generateInstruction(I,I_RETURN_MOV, buf,"","", list);
							if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
						}
					}else{
						goto EXIT;
					}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_int:															//int variable is generated
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_String:														//String variable is generated											
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_double:														//Double variable is generated	
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_while: 
					sprintf(buf, "#while%d",counter);								//while label is generated
					generateInstruction(I,I_LABEL, buf, "", "",list);
					sprintf(continueJump,"#while%d",counter);						//continue jump is saved
					sprintf(breakJump,"#while_end%d",counter);						//break jump is saved						
					int gotoLabel = counter;
					counter++;
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer=runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					sprintf(buf, "#while_end%d",gotoLabel);
					generateInstruction(I,I_WHILE_GOTO, buffer, buf, "",list);		//jump to command block
					sprintf(lastBreakJump, "%s",breakJump);							//save breakJump	
					sprintf(lastContinueJump, "%s",continueJump);					//save continueJump	
					breakable++;													//increment loop depth counter
					lastBreakable=breakable;										//save loop depth
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					sprintf(breakJump, "%s",lastBreakJump);							//restore breakJump
					sprintf(continueJump, "%s",lastContinueJump);					//restore continueJump				
					breakable=lastBreakable;										//restore loop depth
					breakable--;													//decrement loop depth counter
					sprintf(buf, "#while%d",gotoLabel);
					generateInstruction(I,I_GOTO, buf, "", "",list);
					sprintf(buf, "#while_end%d",gotoLabel);
					generateInstruction(I,I_LABEL, buf, "", "",list);
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_do:
					sprintf(buf, "#do%d",counter);									//do label is generated
					generateInstruction(I,I_LABEL, buf, "", "",list);				
					sprintf(continueJump,"#do%d",counter);							//continue jump is saved
					sprintf(breakJump,"#do_end%d",counter);							//break jump is saved	
					counter++;	
					sprintf(lastBreakJump, "%s",breakJump);							//save breakJump	
					sprintf(lastContinueJump, "%s",continueJump);					//save continueJump	
					breakable++;													//increment loop depth counter
					lastBreakable=breakable;										//save loop depth
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}	
					sprintf(breakJump, "%s",lastBreakJump);							//restore breakJump
					sprintf(continueJump, "%s",lastContinueJump);					//restore continueJump				
					breakable=lastBreakable;										//restore loop depth
					breakable--;
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr->type != token_while){fprintf(stderr,"\nWhile at do-while loop expected but not found. \n");goto EXIT;}
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr->type != token_bracketLeftRound){fprintf(stderr,"\n( at do-while loop expected but not found. %d \n",tokenPtr->type);goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * doBuffer=runPrecedenceAnalysis(f,tokenPtr,1,list);
					sprintf(buf2, "#do_end%d",gotoLabel);
					generateInstruction(I,I_DO_GOTO, doBuffer, buf, "",list);		//jump to command block
					generateInstruction(I,I_LABEL, buf2, "", "",list);	
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_if:													 	//if syntax check
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer2=runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					sprintf(buf, "#else%d",counter);
					generateInstruction(I,I_IF_GOTO, buffer2, buf, "",list);
					int counterLast = counter;
					counter++;
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					sprintf(buf, "#cond_end%d",counterLast);
					generateInstruction(I,I_GOTO, buf, "", "",list);
					sprintf(buf, "#else%d",counterLast);
					generateInstruction(I,I_LABEL, buf, "", "",list);
					if ((result=syntaxCheck( ELSE, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nELSE\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					sprintf(buf, "#cond_end%d",counterLast);
					generateInstruction(I,I_LABEL, buf, "", "",list);
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_return:
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr-> type == token_semicolon){
						generateInstruction(I,I_RETURN_NOTHING, "", "", "",list);												//generates instruction for return;
					}else{
						generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
						char* buffer2=runPrecedenceAnalysis(f,tokenPtr,0,list);
						generateInstruction(I,I_RETURN, buffer2, "", "",list);	
						if(tokenPtr-> type != token_semicolon) {fprintf(stderr,"\n;\n");goto EXIT;}
					}
					if ((result=syntaxCheck(FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}		//recursive call
					return result;
				default:
					return -1;
			}

			return result;
			break;



//******************ASSIGN*******************//
		case ASSIGN:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_assign) {				//checks for =
				return 0;
			}else{
				return -1;
			}
			break;

//******************SEMICOLON*******************//
		case SEMICOLON:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_semicolon) {				//checks for ;
				return 0;
			}else{
				return -1;
			}
			break;

//******************LEFT_ROUND*******************//
		case LEFT_ROUND:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketLeftRound) {		//checks for (
				return 0;
			}else{
				return -1;
			}
			break;

//******************RIGHT_ROUND*******************//
		case RIGHT_ROUND:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketRightRound) {		//checks for )
				return 0;
			}else{
				return -1;
			}
			break;
			
//******************LOCAL_VAR_DEC*******************//
		case LOCAL_VAR_DEC:					
			lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));					//save tokens to allocated memory
			if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
			sprintf(buf,"%s",tokenPtr->name);
			sprintf(buf2,"%d",lastToken->type);
			Token *evenLasterToken = memalloc(sizeof(Token));
			evenLasterToken = memcpy(evenLasterToken,tokenPtr,sizeof(Token));
			generateInstruction(I,I_NEW_VAR, buf, buf2,"",list);					//generate instruction for new variable
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr -> type){
				case token_semicolon:												//if semicolon, the variable was not initialized
					return 0;
				case token_assign:													//type id =
					getModifiedToken(f,tokenPtr);
					lastToken=tokenPtr;
					resources->callPtr = memalloc(sizeof(Token));
					resources->callPtr = memcpy(resources->callPtr,tokenPtr,sizeof(Token));
					sprintf(buf3, "%s",lastToken->name);		
					if((result=builtin(f,tokenPtr,list,buf)) == 1){								//check for builtin function call				
						//printType(tokenPtr);					
						return 0;
					}
					//printType(tokenPtr);			
					fflush(stdout);													// Needed to stop fprintf unexpected behaviour.				
					if(tokenPtr-> type == token_identifier){						//	type ID=func
						if(isItFunction(f,tokenPtr) == 1){
							getModifiedToken(f,tokenPtr);
							//printType(tokenPtr);
							if (tokenPtr->type != token_bracketLeftRound){fprintf(stderr,"\n(\n");goto EXIT;}
							if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
							generateInstruction(I,I_FN_CALL, buf3, "","",list);
							sprintf(buf2, "%s",evenLasterToken->name);
							generateInstruction(I,I_RETURN_MOV, buf2,"","", list);
							if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
						}else{														// type ID = EXPRESSION
							generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
							char* buffer =runPrecedenceAnalysis(f,tokenPtr,0,list);
							generateInstruction(I,I_MOV, buf, buffer,"",list);
							if (tokenPtr->type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						}
					}else{			
						generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
						char * buffer=runPrecedenceAnalysis(f,tokenPtr,0,list);
						generateInstruction(I,I_MOV, buf, buffer,"",list);
						if (tokenPtr->type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
					}
					return 0;

				default:
					return -1;
			}
			break;

//******************FN_CALL*******************//
		case FN_CALL:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr ->type){
				case token_bracketRightRound:			//no arguments
					return 0;
					break;
				case token_identifier:					//generate instuction for push
				case token_string:
				case token_intNumber:
				case token_doubleNumber:;
					sprintf(buf3, "%s",resources->callPtr->name);
					//generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer = runPrecedenceAnalysis(f,tokenPtr,0,list);
					generateInstruction(I,I_PUSH,buffer,buf3,"",list);
					if ((result=syntaxCheck( FN_CALL_COMMA, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL_COMMA\n");goto EXIT;}					
					return result;
					break;
				default:
					return -1;
			}

			break;

//******************FN_CALL_COMMA*******************//
		case FN_CALL_COMMA:				
			switch (tokenPtr ->type){
				case token_bracketRightRound:										//end if arguments load
					return 0;
					break;
				case token_comma:;
					//generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer = runPrecedenceAnalysis(f,tokenPtr,1,list);		//next argument
					sprintf(buf3, "%s",resources->callPtr->name);
					generateInstruction(I,I_PUSH,buffer,buf3,"",list);
					if ((result=syntaxCheck( FN_CALL_COMMA, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL_COMMA\n");goto EXIT;}					
					return result;
					break;
				default:
					return -1;
			}

			break;

//******************COMMAND_BLOCK_BEGIN*******************//
		case COMMAND_BLOCK_BEGIN:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr -> type){
				case token_bracketLeftCurly:						//checks for left curly bracket before block of expressions
				if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCB\n");goto EXIT;}
					return result;
					break;
				default:
					return -1;
			}
			break;

//******************COMMAND_BLOCK*******************//
		case COMMAND_BLOCK:			
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr -> type){
				case token_bracketRightCurly:											//end of expression block
					return 0;
					break;
				case token_break:			
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);	
					if(tokenPtr->type != token_semicolon){fprintf(stderr,"\nNo semicolon after break. ");goto EXIT;}	
					if(breakable <= 0){fprintf(stderr,"\nBreak not inside a loop. ");goto EXIT;}						
					sprintf(buf, "%s",breakJump);
					generateInstruction(I,I_GOTO, buf,"","", list);
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
					break;
				case token_continue:
					getModifiedToken(f,tokenPtr);	
					//printType(tokenPtr);	
					if(tokenPtr->type != token_semicolon){fprintf(stderr,"\nNo semicolon after continue. ");goto EXIT;}
					if(breakable <= 0){fprintf(stderr,"\nContinue not inside a loop. ");goto EXIT;}
					sprintf(buf, "%s",continueJump);
					generateInstruction(I,I_GOTO, buf,"","", list);
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}					
					return result;
					break;
				case token_identifier:													//id
					lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));	
					Token *evenLasterToken=memalloc(sizeof(Token));
					evenLasterToken=memcpy(evenLasterToken,tokenPtr,sizeof(Token));
					if(builtin(f,tokenPtr,list, "") == 1){								//check for builtin function call
						if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
						return result;
					}
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr -> type == token_bracketLeftRound){						// id(
						sprintf(buf, "%s",lastToken->name);
						resources->callPtr = lastToken;
						if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
						generateInstruction(I,I_FN_CALL, buf,"","", list);
						if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
					}else if (tokenPtr -> type == token_assign){						//id=
						sprintf(buf, "%s",lastToken->name);
						getModifiedToken(f,tokenPtr);
						if(builtin(f,tokenPtr,list, buf) == 1){							//check for builtin function call
							if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
							return result;
						}
						//printType(tokenPtr);
						if (isItFunction(f,tokenPtr) == 0){								//id=EXPRESSION;
							sprintf(buf, "%s",lastToken->name);
							generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
							char* buffer=runPrecedenceAnalysis(f,tokenPtr,0,list);
							generateInstruction(I,I_MOV, buf,buffer,"", list);
							if (tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						}else{									// id = functionid
							lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));
							getModifiedToken(f,tokenPtr);
							//printType(tokenPtr);
							resources->callPtr = lastToken;
							if(tokenPtr -> type != token_bracketLeftRound){goto EXIT;} 	// id = functionid(
							if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
							sprintf(buf, "%s",lastToken->name);
							generateInstruction(I,I_FN_CALL, buf,"","", list);
							sprintf(buf, "%s",evenLasterToken->name);
							generateInstruction(I,I_RETURN_MOV, buf,"","", list);
							if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
						}
					}else{
						goto EXIT;														//Unexpected token received. Syntax error.
					}
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_while:														//while labels and conditional jumps generation				
					sprintf(buf, "#while%d",counter);
					generateInstruction(I,I_LABEL, buf, "", "",list);	
					sprintf(lastBreakJump, "%s",breakJump);							//save breakJump	
					sprintf(lastContinueJump, "%s",continueJump);					//save continueJump		
					sprintf(continueJump,"#while%d",counter);						//continue jump is saved
					sprintf(breakJump,"#while_end%d",counter);						//break jump is saved					
					int gotoLabel = counter;
					counter++;															//increment counter for unique label identifier
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer=runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}	//end of while expression
					sprintf(buf, "#while_end%d",gotoLabel);
					generateInstruction(I,I_WHILE_GOTO, buffer, buf, "",list);							//recursive call for command block.							
					breakable++;													//increment loop depth counter
					lastBreakable=breakable;										//save loop depth				
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}				
					sprintf(breakJump, "%s",lastBreakJump);							//restore breakJump
					sprintf(continueJump, "%s",lastContinueJump);					//restore continueJump					
					breakable=lastBreakable;										//restore loop depth
					breakable--;													//decrement loop depth counter			
					sprintf(buf, "#while%d",gotoLabel);
					generateInstruction(I,I_GOTO, buf, "", "",list);
					sprintf(buf, "#while_end%d",gotoLabel);
					generateInstruction(I,I_LABEL, buf, "", "",list);									//after the end of expression block, continue with next line in expression block
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_do:
					sprintf(buf, "#do%d",counter);									//do label is generated
					generateInstruction(I,I_LABEL, buf, "", "",list);				
					sprintf(lastBreakJump, "%s",breakJump);							//save breakJump	
					sprintf(lastContinueJump, "%s",continueJump);					//save continueJump	
					sprintf(continueJump,"#do%d",counter);							//continue jump is saved
					sprintf(breakJump,"#do_end%d",counter);							//break jump is saved
					counter++;	
					breakable++;													//increment loop depth counter
					lastBreakable=breakable;										//save loop depth
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}	
					sprintf(breakJump, "%s",lastBreakJump);							//restore breakJump
					sprintf(continueJump, "%s",lastContinueJump);					//restore continueJump				
					breakable=lastBreakable;										//restore loop depth
					breakable--;
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr->type != token_while){fprintf(stderr,"\nWhile at do-while loop expected but not found. \n");goto EXIT;}
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr->type != token_bracketLeftRound){fprintf(stderr,"\n( at do-while loop expected but not found. %d \n",tokenPtr->type);goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * doBuffer=runPrecedenceAnalysis(f,tokenPtr,1,list);
					sprintf(buf2, "#do_end%d",gotoLabel);
					generateInstruction(I,I_DO_GOTO, doBuffer, buf, "",list);		//jump to command block
					generateInstruction(I,I_LABEL, buf2, "", "",list);	
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;	
				case token_if:																			//if labels and conditional jumps generation
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
					char * buffer2=runPrecedenceAnalysis(f,tokenPtr,1,list);							//Precedence analysis is called for expression.
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					sprintf(buf, "#else%d",counter);
					generateInstruction(I,I_IF_GOTO, buffer2, buf, "",list);
					int counterLast = counter;
					counter++;
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					sprintf(buf, "#cond_end%d",counterLast);
					generateInstruction(I,I_GOTO, buf, "", "",list);
					sprintf(buf, "#else%d",counterLast);
					generateInstruction(I,I_LABEL, buf, "", "",list);
					if ((result=syntaxCheck( ELSE, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nELSE\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					sprintf(buf, "#cond_end%d",counterLast);
					generateInstruction(I,I_LABEL, buf, "", "",list);
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_return:																		//function return 
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr-> type == token_semicolon){
						generateInstruction(I,I_RETURN_NOTHING, "", "", "",list);						//no return expression
					}else{
						generateInstruction(I,I_CLEAR_TMPS, "", "", "",list);
						char* buffer2=runPrecedenceAnalysis(f,tokenPtr,0,list);
						generateInstruction(I,I_RETURN, buffer2, "", "",list);							
						if(tokenPtr-> type != token_semicolon) {fprintf(stderr,"\n;\n");goto EXIT;}
					}
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list))	!= 0) {fprintf(stderr,"\nCMB\n");goto EXIT;}
					return result;
				default:
					return -1;
			}
			break;

	

		default:
			fprintf(stderr,"Unexpected state\n");
			return -1;

	EXIT:
		fprintf(stderr, "Syntax Error! \n");
		//printType(tokenPtr);
		fclose(f);
		memfreeall();
		exit(2);
		return result;


	}
	return result;
}
