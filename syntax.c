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

int syntaxCheck (int state, FILE *f,Token* tokenPtr){
	int result;
	switch (state){
		
//***************classBlock*******************//		
		case 0:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_class:
					printType(tokenPtr);
					result=syntaxCheck(1,f,tokenPtr);	//to class
					break;
				case token_EOF:	//TODO check for first token EOF
					printType(tokenPtr);
					fprintf(stderr,"Syntax OK\n");
					break;	
				default:
					fprintf(stderr, "invalid token, class expected, got %d \n",tokenPtr ->type);
					break;
			}		
			return result;
			break;
			
//******************class*******************//
		case 1:
			tokenPtr = getToken(f);
			if (tokenPtr->type == token_identifier){
				printType(tokenPtr);
				result=syntaxCheck(2,f,tokenPtr);	//to className
			}else{ 
				fprintf(stderr, "invalid token, identifier (of class) expected\n");
			}
			return result;
			break;
			
//******************className*******************//			
		case 2:
			tokenPtr = getToken(f);
			if (tokenPtr->type == token_bracketLeftCurly){
				printType(tokenPtr);
				result=syntaxCheck(3,f,tokenPtr);	//to classBody
			}else{
				fprintf(stderr, "invalid token, { expected, got %d\n",tokenPtr-> type);
			}
			return result;
			break;
			
//******************classBody*******************//		
		case 3:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_bracketRightCurly:
					printType(tokenPtr);
					result=syntaxCheck(0,f,tokenPtr);	//back to classBlock
					break;
				case token_static:
					printType(tokenPtr);
					result=syntaxCheck(4,f,tokenPtr);	//to staticDeclaration
					break;
				
				default:			
					fprintf(stderr, "invalid token, got %d\n",tokenPtr-> type);
					break;
			}		
			return result;
			break;
			
//******************staticDeclaration*******************//		
		case 4:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_int:
					printType(tokenPtr);
					result=syntaxCheck(17,f,tokenPtr);	//to staticName
					break;
				case token_double:
					printType(tokenPtr);
					result=syntaxCheck(17,f,tokenPtr);	//to staticName
					break;
				case token_String:
					printType(tokenPtr);
					result=syntaxCheck(17,f,tokenPtr);	//to staticName
					break;
				case token_void:
					printType(tokenPtr);
					result=syntaxCheck(5,f,tokenPtr);	//to functionName		
					break;
				default:	
					printType(tokenPtr);
					fprintf(stderr, "invalid token, got %d, expected type\n",tokenPtr-> type);
					break;
			}		
			return result;
			break;
			
//******************functionName*******************//		
		case 5:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_identifier:
					printType(tokenPtr);
					result=syntaxCheck(6,f,tokenPtr);	//to functionArgumentsLeftBracket
					break;
				
				default:			
					fprintf(stderr, "invalid token, got %d, expected id\n",tokenPtr-> type);
					break;
			}		
			return result;
			break;		
		
//******************functionArgumentsLeftBracket*******************//			
		case 6:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_bracketLeftRound:
					printType(tokenPtr);
					result=syntaxCheck(7,f,tokenPtr);	//to functionArguments
					break;
				default:			
						fprintf(stderr, "invalid token, got %d, expected left round bracket\n",tokenPtr-> type);
						break;
			}
			return result;
			break;
				
//******************functionArguments*******************//			
		case 7:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_bracketRightRound:
					printType(tokenPtr);
					result=syntaxCheck(10,f,tokenPtr);	//to functionBodyLeftBracket
					break;
				case token_int:
					printType(tokenPtr);
					result=syntaxCheck(8,f,tokenPtr);	//to functionArgumentsIdentifier
					break;
				case token_double:
					printType(tokenPtr);
					result=syntaxCheck(8,f,tokenPtr);	//to functionArgumentsIdentifier
					break;		
				case token_String:
					printType(tokenPtr);
					result=syntaxCheck(8,f,tokenPtr);	//to functionArgumentsIdentifier
					break;			
				case token_void:
					printType(tokenPtr);
					result=syntaxCheck(8,f,tokenPtr);	//to functionArgumentsIdentifier
					break;							
				default:			
						fprintf(stderr, "invalid token, got %d, expected type or ) \n",tokenPtr-> type);
						break;
			}	
			return result;
			break;	
				
//******************functionArgumentsIdentifier*******************//			
		case 8:		
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_identifier:
					printType(tokenPtr);
					result=syntaxCheck(9,f,tokenPtr);	//to functionArgumentsComma
					break;
				case token_bracketLeftRound:
					printType(tokenPtr);
					result=syntaxCheck(9,f,tokenPtr);	//to functionBody
					break;	
				default:
					fprintf(stderr, "invalid token, got %d, expected identifier \n",tokenPtr-> type);
					break;
			}
			return result;
			break;
			
//******************functionArgumentsComma*******************//
		case 9:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_comma:
					printType(tokenPtr);
					result=syntaxCheck(7,f,tokenPtr);	//to functionArguments
					break;
				case token_bracketRightRound:
					printType(tokenPtr);
					result=syntaxCheck(11,f,tokenPtr);	//to functionBodyLeftBracket
					break;	
				default:
					fprintf(stderr, "invalid token, got %d, expected identifier \n",tokenPtr-> type);
					break;
				
			}
			return result;
			break;
		
//******************functionArgumentsRightBracket*******************//		
		case 10:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_bracketLeftCurly:
					printType(tokenPtr);
					result=syntaxCheck(11,f,tokenPtr);	//to functionBody
					break;
				default:			
						fprintf(stderr, "invalid token, got %d, expected left curly bracket\n",tokenPtr-> type);
						break;
			}
			return result;
			break;	
				
//******************functionBodyLeftBracket*******************//
		case 11:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_bracketRightCurly:
					printType(tokenPtr);
					result=syntaxCheck(3,f,tokenPtr);	//to classBody
					break;
				case token_int:
					printType(tokenPtr);
					result=syntaxCheck(12,f,tokenPtr);	//to localVarDeclaration
					break;	
				case token_double:
					printType(tokenPtr);
					result=syntaxCheck(12,f,tokenPtr);	//to localVarDeclaration
					break;
				case token_String:
					printType(tokenPtr);
					result=syntaxCheck(12,f,tokenPtr);	//to localVarDeclaration
					break;	
			//	case token_boolean:						//ROZSIRENI
			//		printType(tokenPtr);
			//		result=syntaxCheck(12,f,tokenPtr);	//to localVarDeclaration
			//		break;	
				case token_identifier:
					printType(tokenPtr);
					result=syntaxCheck(15,f,tokenPtr);	//to idInFunction
					break;
				default:			
					fprintf(stderr, "invalid token, got %d, expected }, type, id, function call\n",tokenPtr-> type);
					break;
			}
			return result;
			break;	
	
//******************localVarDeclaration*******************//
		case 12:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_identifier:
					printType(tokenPtr);
					result=syntaxCheck( 13,f,tokenPtr);	//to localVariableDecAssign
					break;			
				default:			
					fprintf(stderr, "invalid token, got %d, expected ID\n",tokenPtr-> type);
					break;
			}
			return result;
			break;	

//******************localVariableDecAssign*******************//
		case 13:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_assign:
					printType(tokenPtr);
					fprintf(stderr, "EXPRESSION HERE\n");	//TODO precedence analysis
					result=syntaxCheck( 14,f,tokenPtr);	//to semicollon
					break;		
				case token_semicolon:
					printType(tokenPtr);				
					result=syntaxCheck( 11,f,tokenPtr);	//to functionBody(leftBracket)
					break;			
				default:			
					fprintf(stderr, "invalid token, got %d, expected = or ; \n",tokenPtr-> type);
					break;
			}
			return result;
			break;
			
//******************semicolon*******************//
		case 14:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_semicolon:
					printType(tokenPtr);
					result=syntaxCheck( 11,f,tokenPtr);	//to functionBody(leftBracket)
					break;			
				default:			
					fprintf(stderr, "invalid token, got %d, expected ; \n",tokenPtr-> type);
					break;
			}
			return result;
			break;

//******************idInFunction*******************//
		case 15:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_dot:
					printType(tokenPtr);
					result=syntaxCheck( 16,f,tokenPtr);	//to globalID.
					break;		
				case token_assign:
					printType(tokenPtr);
					fprintf(stderr, "EXPRESSION HERE\n");	//TODO precedence analysis
					result=syntaxCheck( 14,f,tokenPtr);	//to semicolon
					break;
				default:			
					fprintf(stderr, "invalid token, got %d, expected . or = \n",tokenPtr-> type);
					break;
			}
			return result;
			break;

//******************globalID*******************//
		case 16:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_identifier:
					printType(tokenPtr);
					result=syntaxCheck( 18,f,tokenPtr);	//to globalVarAssign
					break;		

				default:			
					fprintf(stderr, "16: invalid token, got %d, expected ID \n",tokenPtr-> type);
					break;
			}
			return result;
			break;
			
//******************staticName*******************//	
		case 17:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_identifier:	
					printType(tokenPtr);	//TODO EXPRESSION
					result=syntaxCheck( 19,f,tokenPtr);	//to staticFuncOrVar
					break;		
				default:			
					fprintf(stderr, "17: invalid token, got %d, expected = \n",tokenPtr-> type);
					break;
			}
			return result;
			break;			

//******************globalVarAssign*******************//
		case 18:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_assign:
					printType(tokenPtr);		//TODO EXPRESSION
					fprintf(stderr, "EXPRESSION HERE\n");	//TODO precedence analysis
					result=syntaxCheck( 14,f,tokenPtr);	//to semicolon
					break;		

				default:			
					fprintf(stderr, "18: invalid token, got %d, expected = \n",tokenPtr-> type);
					break;
				}
			return result;
			break;

//******************staticFuncOrVar*******************//	
		case 19:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_semicolon:	
					printType(tokenPtr);
					result=syntaxCheck( 3,f,tokenPtr);	//to classBody
					break;		
				case token_bracketLeftRound:	
					printType(tokenPtr);	//TODO EXPRESSION
					result=syntaxCheck( 7,f,tokenPtr);	//function Arguments
					break;	
				case token_assign:	
					printType(tokenPtr);
					fprintf(stderr, "EXPRESSION HERE\n");	//TODO precedence analysis
					result=syntaxCheck( 20,f,tokenPtr);	//to semicolon
					break;		
				default:			
					fprintf(stderr, "19: invalid token, got %d, expected ; or ( or = \n",tokenPtr-> type);
					break;
			}
			return result;
			break;	
//*****************classSemicolon********************//
		case 20:
			tokenPtr = getToken(f);
			switch (tokenPtr ->type){
				case token_semicolon:	
					printType(tokenPtr);
					result=syntaxCheck( 3,f,tokenPtr);	//to classBody
					break;		
	
				default:			
					fprintf(stderr, "20: invalid token, got %d, expected ; \n",tokenPtr-> type);
					break;
			}
			return result;
			break;			
		
		
		
		
		
		
		
		
		
		
		
		
		
		default:		fprintf(stderr, "Syntax Error\n");
			return -1;	
	}
}

int runSyntaxAnalysis (FILE *f) {
	

	Token* tokenPtr = NULL;
	int result = syntaxCheck(0,f,tokenPtr);
	result =result; //TODO delete me
	
	return result;
}






















