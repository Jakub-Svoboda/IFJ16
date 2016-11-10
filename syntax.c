#include "syntax.h"

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

int runSyntaxAnalysis (FILE *f, tListOfInstr * list) {
	Token* lastToken = malloc(sizeof(Token));
	Token* tokenPtr = malloc(sizeof(Token));
	thTable * localVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
	htabInit(localVarTable);
	int result = syntaxCheck(CLASS_BLOCK,f,tokenPtr,lastToken,list,localVarTable);
	result =result; //TODO delete me
	//printHtabLocal(localVarTable);
	

	tInstr I;			//create instruction of end and place it to the end of the instruction list
	generateInstruction(I,I_STOP, NULL, NULL, NULL,list);

	listPrint(list);	
	free(tokenPtr);
	return result;
}

thTable* enterScope(){
	thTable * localVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
	htabInit(localVarTable);
	return 	(localVarTable);	
}		

void generateInstruction(tInstr I, int instType, void *addr1, void *addr2, void *addr3,tListOfInstr* list){	//Adds new instruction to list of instructions
	I.instType = instType;
	I.addr1 = addr1;
	I.addr2 = addr2;
	I.addr3 = addr3;
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

void getModifiedToken(FILE *f,Token* tokenPtr){
	//Token * tmpPtr= getToken(f);
	Token * tmpPtr= lookAhead(f,0);
	memcpy(tokenPtr,tmpPtr,sizeof(Token));
}

void getModifiedLookAhead(FILE *f,Token* tokenPtr){
	Token * tmpPtr= lookAhead(f,0);
	memcpy(tokenPtr,tmpPtr,sizeof(Token));
}

int syntaxCheck (int state, FILE *f,Token* tokenPtr,Token* lastToken, tListOfInstr * list,thTable* localVarTable){
	tInstr I;
	int result=1;	
	static int counter=0; //TODO check if used	// coutner for loop labels
	
	switch (state){

//***************classBlock*******************//
		case CLASS_BLOCK:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if(tokenPtr->type != token_EOF){
					if ((result=syntaxCheck( CLASS, f, tokenPtr, lastToken, list,localVarTable))		!= 0) {goto EXIT;}
					if ((result=syntaxCheck( CLASS_BLOCK, f, tokenPtr, lastToken, list,localVarTable))		!= 0) {goto EXIT;}
			}else{
				fprintf(stderr, "\nSyntax OK\n");
			}
			return 0;
			break;

//******************class*******************//
		case CLASS:
			if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list,localVarTable))					!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
			if ((result=syntaxCheck( LEFT_CURLY_BRACKET, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n{\n");goto EXIT;}
			if ((result=syntaxCheck( CLASS_BODY, f, tokenPtr, lastToken, list,localVarTable))			!= 0) {fprintf(stderr,"\nCB\n");goto EXIT;}
			if ((result=syntaxCheck( RIGHT_CURLY_BRACKET_CURRENT, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n}\n");goto EXIT;}

			return 0;
			break;

//******************ID*******************//
		case ID:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_identifier) {
				return 0;
			}else{
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
				case token_bracketRightCurly:
					return 0;
					break;
				case token_static:
					if ((result=syntaxCheck( TYPE, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\ntype\n");goto EXIT;}
					if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nid\n");goto EXIT;}
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if (tokenPtr -> type == token_assign){
						runPrecedenceAnalysis(f,tokenPtr,1,list);
						if(tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						//printType(tokenPtr);
						break;
					}else{
						if (tokenPtr -> type == token_bracketLeftRound){
							if ((result=syntaxCheck( FUNCTION_DECLARE, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
							break;
						}
					}

					if (tokenPtr -> type == token_semicolon){
						break;
					}
					return result;
					break;
				default:
					return -1;
			}

			if ((result=syntaxCheck( CLASS_BODY, f, tokenPtr, lastToken, list,localVarTable))			!= 0) {fprintf(stderr,"\nCBC\n");goto EXIT;}
			return result;
			break;



//******************FUNCTION_DECLARE*******************//
		case FUNCTION_DECLARE:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if(tokenPtr -> type == token_bracketRightRound){
				if ((result=syntaxCheck( FN_BODY_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFBB\n");goto EXIT;}
			}else{
				if ((result=syntaxCheck( TYPE_CURRENT, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nTYPE\n");goto EXIT;}
				if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list,localVarTable))		!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
				getModifiedToken(f,tokenPtr);
				//printType(tokenPtr);
				if(tokenPtr -> type == token_comma){
					if ((result=syntaxCheck( FUNCTION_DECLARE, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFD\n");goto EXIT;}
				}else{
					if(tokenPtr -> type == token_bracketRightRound){
						if ((result=syntaxCheck( FN_BODY_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFBB\n");goto EXIT;}
					}
				}
			}
			return result;
			break;

//******************TYPE*******************//
		case TYPE:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type == token_int) 	{return 0;}
			if (tokenPtr->type == token_double) {return 0;}
			if (tokenPtr->type == token_String) {return 0;}
			if (tokenPtr->type == token_void) 	{return 0;}


			return -1;
			break;

//******************TYPE_CURRENT*******************//
		case TYPE_CURRENT:
			if (tokenPtr->type == token_int) 	{return 0;}
			if (tokenPtr->type == token_double) {return 0;}
			if (tokenPtr->type == token_String) {return 0;}
			if (tokenPtr->type == token_void) 	{return 0;}


			return -1;
			break;

//******************FN_BODY_BEGIN*******************//
		case FN_BODY_BEGIN:
			if ((result=syntaxCheck( LEFT_CURLY_BRACKET, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nLCB\n");goto EXIT;}
			if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFB\n");goto EXIT;}

			return result;
			break;

//******************FN_BODY*******************//
		case FN_BODY:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch(tokenPtr -> type){
				case token_bracketRightCurly:
					return 0;
					break;
				case token_identifier:								//id
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr -> type == token_bracketLeftRound){	// id(
						if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
						if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}

					}else if (tokenPtr -> type == token_assign){	//id=
						getModifiedToken(f,tokenPtr);
						//printType(tokenPtr);
						if (isItFunction(f,tokenPtr) == 0){		//id=EXPRESSION;
							runPrecedenceAnalysis(f,tokenPtr,0,list);
							if (tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						}else{									// id = functionid
							getModifiedToken(f,tokenPtr);
							//printType(tokenPtr);
							if(tokenPtr -> type != token_bracketLeftRound){goto EXIT;} // id = functionid(
							if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
							if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
						}
					}else{
						goto EXIT;
					}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_int:
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_String:
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_double:
					if ((result=syntaxCheck( LOCAL_VAR_DEC, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nLVD\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					break;
				case token_while:
					generateInstruction(I,I_LABEL, NULL, NULL, NULL,list);
					fprintf(stderr,"I_LABEL: while%d\n",counter);
					int gotoLabel = counter;
					counter++;
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					fprintf(stderr,"I_GOTO: while%d\n",gotoLabel);
					generateInstruction(I,I_GOTO, NULL, NULL, NULL,list);
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_if:
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					if ((result=syntaxCheck( ELSE, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nELSE\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					if ((result=syntaxCheck( FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_return:
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr-> type == token_semicolon){
					}else{
						runPrecedenceAnalysis(f,tokenPtr,0,list);
						if(tokenPtr-> type != token_semicolon) {fprintf(stderr,"\n;\n");goto EXIT;}
						}
					if ((result=syntaxCheck(FN_BODY, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				default:
					return -1;
			}
			//if ((result=syntaxCheck( FN_BODY_CURRENT, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFBC\n");goto EXIT;}

			return result;
			break;



//******************ASSIGN*******************//
		case ASSIGN:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_assign) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************SEMICOLON*******************//
		case SEMICOLON:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_semicolon) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************LEFT_ROUND*******************//
		case LEFT_ROUND:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketLeftRound) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************RIGHT_ROUND*******************//
		case RIGHT_ROUND:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_bracketRightRound) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************BREAK*******************//
		case BREAK:
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			if (tokenPtr->type== token_break) {
				return 0;
			}else{
				return -1;
			}
			break;

//******************LOCAL_VAR_DEC*******************//
		case LOCAL_VAR_DEC:
			lastToken =memcpy(lastToken,tokenPtr,sizeof(Token));		
			if ((result=syntaxCheck( ID, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nID\n");goto EXIT;}
			generateInstruction(I,I_NEW_VAR, tokenPtr, lastToken, NULL,list);				//TODO check if not redeclaration
			fprintf(stderr,"I_NEW_VAR: %s, %d\n",tokenPtr->name, lastToken->type);
			getModifiedToken(f,tokenPtr);
			//printType(tokenPtr);
			switch (tokenPtr -> type){
				case token_semicolon:
					return 0;
				case token_assign:
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
						if(tokenPtr-> type == token_identifier){
							if(isItFunction(f,tokenPtr) == 1){
								getModifiedToken(f,tokenPtr);
								//printType(tokenPtr);
								if (tokenPtr->type != token_bracketLeftRound){fprintf(stderr,"\n(\n");goto EXIT;}
								if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
								if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
							}else{
								runPrecedenceAnalysis(f,tokenPtr,0,list);
								if (tokenPtr->type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
							}
						}else{
							runPrecedenceAnalysis(f,tokenPtr,0,list);
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
				case token_bracketRightRound:
					fprintf(stderr,"I_FN_CALL\n");
					//generateInstruction(I,I_GOTO, NULL, NULL, NULL,list);
					return 0;
					break;
				case token_identifier:
				case token_string:
				case token_intNumber:
				case token_doubleNumber:
					runPrecedenceAnalysis(f,tokenPtr,0,list);
					if ((result=syntaxCheck( FN_CALL_COMMA, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL_COMMA\n");goto EXIT;}					
					return result;
					break;
				default:
					return -1;
			}

			break;

//******************FN_CALL_COMMA*******************//
		case FN_CALL_COMMA:				
			switch (tokenPtr ->type){
				case token_bracketRightRound:
					fprintf(stderr,"I_FN_CALL\n");
					return 0;
					break;
				case token_comma:
					runPrecedenceAnalysis(f,tokenPtr,1,list);
					if ((result=syntaxCheck( FN_CALL_COMMA, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL_COMMA\n");goto EXIT;}					
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
				case token_bracketLeftCurly:
				if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCB\n");goto EXIT;}
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
				case token_bracketRightCurly:
					return 0;
					break;
				case token_identifier:								//id
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr -> type == token_bracketLeftRound){	// id(
						if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
						if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}

					}else if (tokenPtr -> type == token_assign){	//id=
						getModifiedToken(f,tokenPtr);
						//printType(tokenPtr);
						if (isItFunction(f,tokenPtr) == 0){		//id=EXPRESSION;
							runPrecedenceAnalysis(f,tokenPtr,0,list);
							if (tokenPtr -> type != token_semicolon){fprintf(stderr,"\n;\n");goto EXIT;}
						}else{									// id = functionid
							getModifiedToken(f,tokenPtr);
							//printType(tokenPtr);
							if(tokenPtr -> type != token_bracketLeftRound){goto EXIT;} // id = functionid(
							if ((result=syntaxCheck( FN_CALL, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFN_CALL\n");goto EXIT;}
							if ((result=syntaxCheck( SEMICOLON, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n;\n");goto EXIT;}
						}
					}else{
						goto EXIT;
					}
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCMB\n");goto EXIT;}
					break;
				case token_while:
					generateInstruction(I,I_LABEL, NULL, NULL, NULL,list);
					fprintf(stderr,"I_LABEL: while%d\n",counter);
					int gotoLabel = counter;
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr -> type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					counter++;
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					fprintf(stderr,"I_GOTO: while%d\n",gotoLabel);
					generateInstruction(I,I_GOTO, NULL, NULL, NULL,list);
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_if:
					if ((result=syntaxCheck( LEFT_ROUND, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\n(\n");goto EXIT;}
					runPrecedenceAnalysis(f,tokenPtr,1,list);
					if(tokenPtr->type != token_bracketRightRound){fprintf(stderr,"\n)\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					if ((result=syntaxCheck( ELSE, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nELSE\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK_BEGIN, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCBB\n");goto EXIT;}
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nFNB\n");goto EXIT;}
					return result;
				case token_return:
					getModifiedToken(f,tokenPtr);
					//printType(tokenPtr);
					if(tokenPtr-> type == token_semicolon){
					}else{
						runPrecedenceAnalysis(f,tokenPtr,0,list);
						if(tokenPtr-> type != token_semicolon) {fprintf(stderr,"\n;\n");goto EXIT;}
					}
					if ((result=syntaxCheck( COMMAND_BLOCK, f, tokenPtr, lastToken, list,localVarTable))	!= 0) {fprintf(stderr,"\nCMB\n");goto EXIT;}
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
		exit(2);
		return result;


	}
	return result;
}
