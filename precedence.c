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
//Returns int representing the reduction rule. Also pops the whole rule out of stack. Good luck reverse engineering this.
void printStack(tStack* s){
		int i=0;
		while(i<=s->top){
			fprintf(stderr,"%d ", s->arr[i]->type);
			i++;
		}
		fprintf(stderr,"\n");
	}

Token* getModifiedTokenPrecedence(FILE *f,Token* tokenPtr){
	//Token * tmpPtr= getToken(f);
	Token * tmpPtr= lookAhead(f,0);
	memcpy(tokenPtr,tmpPtr,sizeof(Token));
	return tmpPtr;
}

Token* whatRule(tStack* stack, tListOfInstr * list){
	static int tmpCounter = 0;
	Token* tokenPtr = memalloc(sizeof(Token));
	char buf[2047];
	char buf2[2047];
	//char buf3[2047];
	tInstr I;
	tokenPtr = stackTop(stack);				//read top of the stack
	stackPop(stack);						//pop the token we dont need
	Token * lastToken;
	Token * toBePushedE = memalloc(sizeof(Token));		//TODO sort of a memory waste
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
						//fprintf(stderr,"1st op is: %s\n",lastToken->name);
						//fprintf(stderr,"2nd op is: %s\n",lastToken2->name);
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
						exit(2);//TODO improve?
						break;

				}
				break;
			case token_identifier:											//2nd token is i
				//sprintf(buf, "_var%d",tmpCounter);
				//sprintf(buf2, "%d",lastToken->type);
				//generateInstruction(I,I_NEW_VAR, buf, buf2, "",list);
				//tmpCounter++;
				lastToken=tokenPtr;
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){				
					// rule 12 <i>
					strcpy(toBePushedE->name,lastToken->name);
					toBePushedE->type = token_expression;
					
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
					//printStack(stack);
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
					//printStack(stack);
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
					//printStack(stack);
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
					//printStack(stack);
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
				exit(2);//TODO improve?
				break;

		}
	}else{
		fprintf(stderr,"First token to be reduced is not right handle\n");


	}

	//fprintf(stderr,"rule is: %d\n\n",rule);

	return toBePushedE;

}

Token* stackTopTerminal(tStack* s){	//Returns the top terminal on stack
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

Token* reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack, tListOfInstr * list){
	Token *toBePushed = memalloc(sizeof(Token));
	while(1){
		stackTopPtr=stackTopTerminal(stack);	//find first Terminal on stack
		char whatToDo;	//This deals with how to access the precedence table when number or string gets on input/ stack top

		if ((tokenPtr -> type) == token_intNumber || (tokenPtr -> type) == token_doubleNumber || (tokenPtr -> type) == token_string){
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
	//	printf("input is:    %d\n", tokenPtr->type);					//TODO test-output,delete later
	//	printf("stack top is %d\n", stackTopPtr->type);
	//	printf("What to do is: %c\n\n",whatToDo);
		if(whatToDo == '$'){
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
				//printStack(stack);

			}else{
				toBePushed -> type = token_leftHandle;		//IF E is not top token
				stackPush(stack, toBePushed);				//push left handle and input TOken
				stackPush(stack, tokenPtr);
			}
				return NULL;
			//printStack(stack);

			break;											//Break the cycle to get new token
		}
		if(whatToDo == '>'){				//IF precedence table returns >, we reduce
			toBePushed -> type = token_rightHandle;			//Close the rule with right handle
			stackPush(stack,toBePushed);
			//printStack(stack);	//TODO delete
			Token * toBePushedE = whatRule(stack, list);	//Find out what rule applies and pop the rule out of stack
			if (tokenPtr->type != token_semicolon){	//NOT Semicolon
				stackPush(stack, toBePushedE);
				//printStack(stack);
				}else{
				if(stack->top == 0 && stack->arr[stack->top]->type == token_dollar){	//semicolon on input
					fprintf(stderr,"Predictive syntax analysis is over\n");		//TODO return
					//printStack(stack);
					return NULL;
					break;
				}else{
					fprintf(stderr,"Semicolon on input but stack not empty! \n");
				}
			}
		}
		if(whatToDo == '='){
			stackPush(stack,tokenPtr);
			toBePushed -> type = token_rightHandle;			//Close the rule with right handle
			stackPush(stack,toBePushed);
			//printStack(stack);
			Token * toBePushedE = whatRule(stack,list);	//TODO assign somewhere		//Find out what rule applies and pop the rule out of stack
			//printStack(stack);
			 
			toBePushedE->type = token_expression;		// push E
			stackPush(stack, toBePushedE);
			break;
		}
		if(whatToDo == '0'){
			fprintf(stderr,"Syntax Error\n");	//TODO improve?
			memfreeall();
			exit(2);

		}
	}
	return NULL; 	//TODO check if ok
	//free(toBePushed);
}

void stackPush(tStack* s,Token* Token){
	s->top=s->top+1;
	s->arr[s->top]=Token;
//	Token* stackTop= stackTop(s);
//	fprintf(stderr,"A token has been pushed: %d, s Top is: %d\n",Token ->type,s->top);		//TODO test-output,delete later
}

void stackPop(tStack* s){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackPop requested but stack is empty.\n");
	}else{
		s->arr[s->top] = NULL ;
		(s->top)--;
	}
}

Token* stackTop(tStack* s){
	Token* stackTopPtr=NULL;
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackTop requested but stack is empty.\n");
	}else{
		stackTopPtr = s->arr[s->top];
	}
	return stackTopPtr;
}

int stackEmpty (tStack* s){
	return(-1 == s->top);
}

char* runPrecedenceAnalysis(FILE* f,Token *tokenPtr,int readFirst, tListOfInstr * list){
	tStack* stack=memalloc(sizeof(tStack));	//initialize stack
	stack->top=-1;
	Token* tokenPtrTmp = tokenInit();			//init for the first $
	tokenPtrTmp->type=token_dollar;
	stackPush(stack,tokenPtrTmp);
	Token* stackTopPtr=stackTop(stack);	//initialize stack pointer
	int tokenCnt =0;			//if no expression is present, this causes error
	int depth=0;	//Defines how many brackets are yet to come to finish expression
	Token * lastVarPtr=NULL;
	while(1){
		tokenCnt++;
		Token * precedencePtr;
		if (readFirst == 0){
			readFirst++;
			precedencePtr = memalloc(sizeof(Token));
			memcpy(precedencePtr,tokenPtr,sizeof(Token));
		}else{
			precedencePtr = getModifiedTokenPrecedence(f,tokenPtr);
		}
		if(tokenPtr->type>token_identifier && tokenPtr->type != token_comma && tokenPtr->type!=token_semicolon && tokenPtr->type!=token_intNumber && tokenPtr->type!=token_doubleNumber && tokenPtr->type!=token_string && tokenPtr->type!=token_bracketLeftRound && tokenPtr->type!=token_bracketRightRound)
		{
			fprintf(stderr,"syntax error, unexpected token %d\n",tokenPtr->type);
			memfreeall();
			exit(2);	//TODO free all stuff
		}

		if (tokenPtr->type==token_semicolon || tokenPtr->type == token_comma){
			if(tokenCnt==1){fprintf(stderr,"Syntax Error, expression expected.\n"); memfreeall(); exit(2);}
			stackTopPtr=stackTopTerminal(stack);
			//fprintf(stderr,"Stak top ptr is: %d\n",tokenPtr->type);
			Token* tokenPtrTmp = tokenInit();			//init for the last $
			tokenPtrTmp->type = token_dollar;
			lastVarPtr=reduction(tokenPtrTmp,stackTopPtr,stack,list);
	//	fprintf(stderr,"lastVarPtr->name: %s\n",lastVarPtr->name);	
			return lastVarPtr->name;
		}
		if (tokenPtr->type == token_bracketLeftRound){depth++;}
		if (tokenPtr->type == token_bracketRightRound){
			if(tokenCnt==1){fprintf(stderr,"Syntax Error, expression expected.\n"); memfreeall(); exit(2);}
			depth--;
			if (depth<0){
				Token* tokenPtrTmp = tokenInit();			//init for the last $
				tokenPtrTmp->type = token_dollar;
				lastVarPtr=reduction(tokenPtrTmp,stackTopPtr,stack,list);
				//fprintf(stderr,"Predictive syntax analysis over, returning ) to recursive analysis %d\n",depth); //TODO return!!!!!!!!
	//	fprintf(stderr,"lastVarPtr->name: %s\n",lastVarPtr->name);
				return lastVarPtr->name;
			}
		}
		stackTopPtr = stackTopTerminal(stack);
		lastVarPtr=reduction(precedencePtr, stackTopPtr, stack,list);
//fprintf(stderr,"lastVarPtr->name: %s\n",lastVarPtr->name);
	}
	//free(tokenPtrTmp2);
	//free(stack);
	return 0;
}
