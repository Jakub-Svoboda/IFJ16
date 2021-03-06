/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    precedence.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "syntax.h"
#include "precedence.h"

/*Table for determining which action to take according to stack top and input */
char precedence_table[14][14]={
//input
//+   -   *   /   <   >  <=  >=  ==  !=	  (	  )   i   $
{'>','>','<','<','>','>','>','>','>','>','<','>','<','>'},	// +
{'>','>','<','<','>','>','>','>','>','>','<','>','<','>'},	// -
{'>','>','>','>','>','>','>','>','>','>','<','>','<','>'},	// *
{'>','>','>','>','>','>','>','>','>','>','<','>','<','>'},	// /
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//<
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//>
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//<=
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//>=
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//==
{'<','<','<','<','>','>','>','>','>','>','<','>','<','>'},	//!=
{'<','<','<','<','<','<','<','<','<','<','<','=','<','0'},	//(
{'>','>','>','>','>','>','>','>','>','>','0','>','0','>'},	//)
{'>','>','>','>','>','>','>','>','>','>','0','>','0','>'},	//i
{'<','<','<','<','<','<','<','<','<','<','<','0','<','$'},	//$       	stack
};
//Returns int representing the reduction rule. Also pops the whole rule out of stack. 

/*Debugging function for printing stack*/
void printStack(tStack* s){
		int i=0;
		while(i<=s->top){					//prints all token types from stack in cycle
			fprintf(stderr,"%d ", s->arr[i]->type);
			i++;
		}
		fprintf(stderr,"\n");
	}

/*Copies new pointer to the place pointed to by other pointers*/
Token* getModifiedTokenPrecedence(FILE *f,Token* tokenPtr){
	//Token * tmpPtr= getToken(f);
	Token * tmpPtr= lookAhead(f,0);
	memcpy(tokenPtr,tmpPtr,sizeof(Token));
	return tmpPtr;
}

/*Function which determines rule for stack terminals reduction*/
Token* whatRule(tStack* stack, tListOfInstr * list){
	static int tmpCounter = 0;
	Token* tokenPtr = memalloc(sizeof(Token));
	char buf[2047];
	char buf2[2047];
	tInstr I;
	tokenPtr = stackTop(stack);				//read top of the stack
	stackPop(stack);						//pop the token we dont need
	Token * lastToken;
	Token * toBePushedE = memalloc(sizeof(Token));
	toBePushedE->name=memalloc(sizeof(char)*2047);
	
	if((tokenPtr -> type)==token_rightHandle){
		tokenPtr = stackTop(stack);							//read top of the stack
		stackPop(stack);													//pop the token we dont need
		Token * lastToken2 = tokenPtr;
		
		switch (tokenPtr -> type){
			case token_expression:											//2nd token is E
				tokenPtr = stackTop(stack);					//read top of the stack
				stackPop(stack);											//pop the token we dont need
				switch (tokenPtr -> type){									// E>
					case token_add:											// +E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E+E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_ADD, buf, lastToken->name, lastToken2->name, list);
								//rule 1 <E+E>
							}else{
								fprintf(stderr,"Error: Someting E+E>\n");
							}
						}
						break;

					case token_subtract:										// -E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E-E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_SUB, buf, lastToken->name, lastToken2->name, list);
								//rule 2 <E-E>
							}else{
								fprintf(stderr,"Error: Someting E-E>\n");
							}
						}
						break;

					case token_multiply:										// *E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E*E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_MUL, buf, lastToken->name, lastToken2->name, list);
								//rule 3 <E*E>
							}else{
								fprintf(stderr,"Error: Someting E*E>\n");
							}
						}
						break;

					case token_divide:										// /E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E/E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_DIV, buf, lastToken->name, lastToken2->name, list);
								//rule 4 <E/E>
							}else{
								fprintf(stderr,"Error: Someting E/E>\n");
							}
						}
						break;

					case token_less:										// <E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E<E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_LT, buf, lastToken->name, lastToken2->name, list);
								//rule 5 <E<E>
							}else{
								fprintf(stderr,"Error: Someting E<E>\n");
							}
						}
						break;

					case token_greater:										// >E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){				// E>E>
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_GT, buf, lastToken->name, lastToken2->name, list);
								//rule 6 <E>E>
							}else{
								fprintf(stderr,"Error: Someting E>E>\n");
							}
						}
						break;

					case token_lessEqual:										// ==E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_LE, buf, lastToken->name, lastToken2->name, list);
								//rule 7 <E<=E>
							}else{
								fprintf(stderr,"Error: Someting E<=E>\n");
							}
						}
						break;

					case token_greaterEqual:										// >=E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_GE, buf, lastToken->name, lastToken2->name, list);
								//rule 8 <E>=E>
							}else{
								fprintf(stderr,"Error: Someting E>=E>\n");
							}
						}
						break;

					case token_equal:										// ==E>
						tokenPtr = stackTop(stack);
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_EQ, buf, lastToken->name, lastToken2->name, list);
								//rule 9 <E==E>
							}else{
								fprintf(stderr,"Error: Someting E!=E>\n");
							}
						}
						break;

					case token_notEqual:
						tokenPtr = stackTop(stack);			//read top of the stack
						stackPop(stack);
						lastToken=tokenPtr;
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);		//read top of the stack
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								sprintf(buf, "#var%d",tmpCounter);
								strcpy(toBePushedE->name,buf);
								toBePushedE->type = token_expression;
								generateInstruction(I,I_NEW_VAR, buf, "", "",list);
								tmpCounter++;
								generateInstruction(I,I_NE, buf, lastToken->name, lastToken2->name, list);
								// rule 10 <E!=E>
							}else{
								fprintf(stderr,"Error: Someting E!=E>\n");
							}
						}
						break;

					default:
						fprintf(stderr,"Unexpected token\n");
						memfreeall();
						exit(2);
						break;

				}
				break;
			case token_identifier:											//2nd token is i
				lastToken=tokenPtr;
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){				
					// rule 12 <i>
					strcpy(toBePushedE->name,lastToken->name);
					toBePushedE->type = token_expression;
					
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
				}
				break;
			case token_intNumber:
				sprintf(buf, "#var%d",tmpCounter);
				sprintf(buf2, "%d",28);
				generateInstruction(I,I_NEW_VAR, buf, buf2, "",list);
				sprintf(buf2, "%s",tokenPtr->name);
				generateInstruction(I,I_MOV_INT,buf,buf2,"",list);
				tmpCounter++;
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){		
					strcpy(toBePushedE->name,buf);				
					toBePushedE->type = token_expression;
					// rule 12 <i>
					
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
				}
				break;
			case token_doubleNumber:
				sprintf(buf, "#var%d",tmpCounter);
				sprintf(buf2, "%d",23);
				generateInstruction(I,I_NEW_VAR, buf, buf2, "",list);
				sprintf(buf2, "%s",tokenPtr->name);
				generateInstruction(I,I_MOV_DOUBLE,buf,buf2,"",list);
				tmpCounter++;
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){
					strcpy(toBePushedE->name,buf);
					toBePushedE->type = token_expression;
					// rule 12 <i>
					
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
				}
				break;
			case token_string:
				sprintf(buf, "#var%d",tmpCounter);
				sprintf(buf2, "%d",30);
				generateInstruction(I,I_NEW_VAR, buf, buf2, "",list);
				sprintf(buf2, "%s",tokenPtr->name);
				generateInstruction(I,I_MOV_STRING,buf,buf2,"",list);
				tmpCounter++;
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){
					strcpy(toBePushedE->name,buf);
					toBePushedE->type = token_expression;
					// rule 12 <i>
					
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
				}
				break;
			case token_bracketRightRound:
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				lastToken=tokenPtr;
				if( tokenPtr -> type  == token_expression){
					tokenPtr = stackTop(stack);					//read top of the stack
					stackPop(stack);										//pop the token we dont need
					if( tokenPtr -> type  == token_bracketLeftRound){
						tokenPtr = stackTop(stack);				//read top of the stack
						stackPop(stack);									//pop the token we dont need
						if(tokenPtr -> type  == token_leftHandle){
							strcpy(toBePushedE->name,lastToken->name);
							toBePushedE->type = token_expression;	
							//rule 11 <(E)>
						}
					}
				}
				break;
			default:
				fprintf(stderr,"Unexpected token\n");
				memfreeall();
				exit(2);
				break;

		}
	}else{
		fprintf(stderr,"First token to be reduced is not right handle\n");


	}

	return toBePushedE;

}

/*Function returning the top terminal on stack*/
Token* stackTopTerminal(tStack* s){
	Token* stackTopPtr=NULL;
	int i = s->top;
	stackTopPtr = s->arr[i];		// i is assigned TOP
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackTop requested but stack is empty.\n");
	}else{
		while(stackTopPtr -> type == token_leftHandle || stackTopPtr -> type == token_expression){
			stackTopPtr = s->arr[i];
			i--;
			if(i<0){
				break;
			}
		}
	}
	return stackTopPtr;
}

/*Function which pops terminals from stack and pushes nonterminal to stack=>reduction according to precedence rules*/
Token* reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack, tListOfInstr * list){
	Token *toBePushed = memalloc(sizeof(Token));
	while(1){
		stackTopPtr=stackTopTerminal(stack);	//find first Terminal on stack
		char whatToDo;	//This deals with how to access the precedence table when number or string gets on input/ stack top

		if ((tokenPtr -> type) == token_intNumber || (tokenPtr -> type) == token_doubleNumber || (tokenPtr -> type) == token_string){					//if exact value entered, convert to identifier
			if ((stackTopPtr-> type ) == token_intNumber || (stackTopPtr -> type) == token_doubleNumber || (stackTopPtr -> type) == token_string){
				whatToDo = precedence_table[token_identifier][ token_identifier];
			}else{
				whatToDo = precedence_table[stackTopPtr -> type][ token_identifier];
			}
		}else{
			if ((stackTopPtr-> type ) == token_intNumber || (stackTopPtr -> type) == token_doubleNumber || (stackTopPtr -> type) == token_string){
				whatToDo = precedence_table[token_identifier][ tokenPtr->type];
			}else{
				whatToDo = precedence_table[stackTopPtr->type][tokenPtr->type];
			}
		}
		if(whatToDo == '$'){					//if dollar, end precedence
			tokenPtr=stackTop(stack);
			stackPop(stack);	
			return tokenPtr;
		}
		if(whatToDo == '<'){				//When table returns <
			stackTopPtr=stackTop(stack);	//check what is the very top of stack
			if (stackTopPtr->type == token_expression){		//if E is on top
				Token* tmpPtr = stackTopPtr;				//then we pop it and save it to temporary ptr
				stackPop(stack);
				toBePushed -> type = token_leftHandle;		//push left handle
				stackPush(stack, toBePushed);
				stackPush(stack, tmpPtr);					//push E back from temporary ptr
				stackPush(stack, tokenPtr);					//push input token

			}else{
				toBePushed -> type = token_leftHandle;		//IF E is not top token
				stackPush(stack, toBePushed);				//push left handle and input TOken
				stackPush(stack, tokenPtr);
			}
				return NULL;

			break;											//Break the cycle to get new token
		}
		if(whatToDo == '>'){				//IF precedence table returns >, we reduce
			toBePushed -> type = token_rightHandle;			//Close the rule with right handle
			stackPush(stack,toBePushed);
			Token * toBePushedE = whatRule(stack, list);	//Find out what rule applies and pop the rule out of stack
			if (tokenPtr->type != token_semicolon){	//NOT Semicolon
				stackPush(stack, toBePushedE);
				}else{
				if(stack->top == 0 && stack->arr[stack->top]->type == token_dollar){	//semicolon on input
					fprintf(stderr,"Predictive syntax analysis is over\n");	
					return NULL;
					break;
				}
			}
		}
		if(whatToDo == '='){
			stackPush(stack,tokenPtr);
			toBePushed -> type = token_rightHandle;			//Close the rule with right handle
			stackPush(stack,toBePushed);
			Token * toBePushedE = whatRule(stack,list);			//Find out what rule applies and pop the rule out of stack
			 
			toBePushedE->type = token_expression;		// push E
			stackPush(stack, toBePushedE);
			break;
		}
		if(whatToDo == '0'){
			fprintf(stderr,"Syntax Error\n");
			memfreeall();
			exit(2);

		}
	}
	return NULL; 	
}

/*Function for pushing tokens to stack*/
void stackPush(tStack* s,Token* Token){					
	s->top=s->top+1;									//change the top index
	s->arr[s->top]=Token;								//push token
}

/*Function which pops tokens from stack*/
void stackPop(tStack* s){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackPop requested but stack is empty.\n");
	}else{
		s->arr[s->top] = NULL ;											//erase top of the stack 
		(s->top)--;														//change position of stack top
	}
}

/*Function returning address of top stack Token*/
Token* stackTop(tStack* s){
	Token* stackTopPtr=NULL;
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackTop requested but stack is empty.\n");
	}else{
		stackTopPtr = s->arr[s->top];									//store stack top in pointer
	}
	return stackTopPtr;													//and return that pointer to caller
}

/*Function for checking if stack is empty*/
int stackEmpty (tStack* s){
	return(-1 == s->top);
}

/*Function which handles precedence analysis, this function is called from syntax.c when recursive syntax analysis finds expression*/
char* runPrecedenceAnalysis(FILE* f,Token *tokenPtr,int readFirst, tListOfInstr * list){
	tStack* stack=memalloc(sizeof(tStack));		//initialize stack
	stack->top=-1;
	Token* tokenPtrTmp = tokenInit();			//init for the first $
	tokenPtrTmp->type=token_dollar;
	stackPush(stack,tokenPtrTmp);
	Token* stackTopPtr=stackTop(stack);			//initialize stack pointer
	int tokenCnt =0;							//if no expression is present, this causes error
	int depth=0;								//Defines how many brackets are yet to come to finish expression
	Token * lastVarPtr=NULL;
	while(1){
		tokenCnt++;
		Token * precedencePtr;
		if (readFirst == 0){					//flag for determining if first token is already loaded, if it is loaded, copy it, otherwise load it
			readFirst++;
			precedencePtr = memalloc(sizeof(Token));
			memcpy(precedencePtr,tokenPtr,sizeof(Token));
		}else{
			precedencePtr = getModifiedTokenPrecedence(f,tokenPtr);
		}
		if(tokenPtr->type>token_identifier && tokenPtr->type != token_comma && tokenPtr->type!=token_semicolon && tokenPtr->type!=token_intNumber && tokenPtr->type!=token_doubleNumber && tokenPtr->type!=token_string)	//check for token type
		{
			fprintf(stderr,"syntax error, unexpected token %d\n",tokenPtr->type);
			memfreeall();
			exit(2);
		}

		if (tokenPtr->type==token_semicolon || tokenPtr->type == token_comma){										//if expression ends, precedence analysis is over
			if(tokenCnt==1){fprintf(stderr,"Syntax Error, expression expected.\n"); memfreeall(); exit(2);}
			stackTopPtr=stackTopTerminal(stack);
			Token* tokenPtrTmp = tokenInit();			//init for the last $
			tokenPtrTmp->type = token_dollar;
			lastVarPtr=reduction(tokenPtrTmp,stackTopPtr,stack,list);
			return lastVarPtr->name;
		}
		if (tokenPtr->type == token_bracketLeftRound){depth++;}				//if left bracket is found, we increment depth variable, which is used for checking if number of L and R brackets is equal
		if (tokenPtr->type == token_bracketRightRound){
			if(tokenCnt==1){fprintf(stderr,"Syntax Error, expression expected.\n"); memfreeall(); exit(2);}		//if expression is empty=>error
			depth--;
			if (depth<0){
				Token* tokenPtrTmp = tokenInit();			//init for the last $
				tokenPtrTmp->type = token_dollar;
				lastVarPtr=reduction(tokenPtrTmp,stackTopPtr,stack,list);
				return lastVarPtr->name;
			}
		}
		stackTopPtr = stackTopTerminal(stack);
		lastVarPtr=reduction(precedencePtr, stackTopPtr, stack,list);
	}
	return 0;
}
