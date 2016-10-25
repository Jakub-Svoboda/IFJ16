#include "syntax.h"

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
{'<','<','<','<','<','<','<','<','<','<','<','0','<','0'},	//$       	stack
};
//Returns int representing the reduction rule. Also pops the whole rule out of stack. Good luck reverse engineering this.
int whatRule(tStack* stack){
	int rule;	
	tStack* bufferStack = malloc(sizeof(tStack));				//temporar stack where we push the tokens to be reduced
	Token* tokenPtr = malloc(sizeof(Token));
	
	tokenPtr = stackTop(stack);				//read top of the stack
	stackPop(stack);						//pop the token we dont need
	
	if( (tokenPtr -> type)==token_rightHandle){
		tokenPtr = stackTop(stack);							//read top of the stack
		stackPop(stack);													//pop the token we dont need
		switch (tokenPtr -> type){
			case token_expression:											//2nd token is E
				tokenPtr = stackTop(stack);					//read top of the stack
				stackPop(stack);											//pop the token we dont need
				
				switch (tokenPtr -> type){									// E>
				
					case token_add:											// +E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E+E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 1;									//rule 1 <E+E>
							}else{
								fprintf(stderr,"Error: Someting E+E>\n");
							}
						}
						break;
					
					case token_subtract:										// -E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E-E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 2;									//rule 2 <E-E>
							}else{
								fprintf(stderr,"Error: Someting E-E>\n");
							}
						}
						break;
					
					case token_multiply:										// *E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E*E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 3;									//rule 3 <E*E>
							}else{
								fprintf(stderr,"Error: Someting E*E>\n");
							}
						}
						break;
					
					case token_divide:										// /E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E/E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 4;									//rule 4 <E/E>
							}else{
								fprintf(stderr,"Error: Someting E/E>\n");
							}
						}
						break;
				
					case token_less:										// <E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E<E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 5;									//rule 5 <E<E>
							}else{
								fprintf(stderr,"Error: Someting E<E>\n");
							}
						}
						break;
					
					case token_greater:										// >E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){				// E>E>
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 6;									//rule 6 <E>E>
							}else{
								fprintf(stderr,"Error: Someting E>E>\n");
							}
						}
						break;
									
					case token_lessEqual:										// ==E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 7;									//rule 7 <E<=E>
							}else{
								fprintf(stderr,"Error: Someting E<=E>\n");
							}
						}
						break;
								
					case token_greaterEqual:										// >=E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 8;									//rule 8 <E>=E>
							}else{
								fprintf(stderr,"Error: Someting E>=E>\n");
							}
						}
						break;
										
					case token_equal:										// ==E>
						tokenPtr = stackTop(stack);			
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);		
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 9;									//rule 9 <E!=E>
							}else{
								fprintf(stderr,"Error: Someting E!=E>\n");
							}
						}
						break;	
								
					case token_notEqual:	
						tokenPtr = stackTop(stack);			//read top of the stack
						stackPop(stack);
						if(tokenPtr -> type  == token_expression){
							tokenPtr = stackTop(stack);		//read top of the stack
							stackPop(stack);
							if(tokenPtr -> type  == token_leftHandle){
								rule = 10;									// rule 10 <E!=E>
							}else{
								fprintf(stderr,"Error: Someting E!=E>\n");
							}
						}
						break;	
						
					default:
						fprintf(stderr,"Unexpected token\n");
						break;
						
				}
				break;
			case token_identifier:											//2nd token is i	
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if (tokenPtr -> type  == token_leftHandle){
					rule= 12;												// rule 12 <i>
				}else{
					fprintf(stderr,"Error: %d i >\n\n",tokenPtr ->type);
				}
				break;
			case token_bracketRightRound:	
				tokenPtr = stackTop(stack);						//read top of the stack
				stackPop(stack);											//pop the token we dont need
				if( tokenPtr -> type  == token_expression){
					tokenPtr = stackTop(stack);					//read top of the stack
					stackPop(stack);										//pop the token we dont need
					if( tokenPtr -> type  == token_bracketLeftRound){
						tokenPtr = stackTop(stack);				//read top of the stack
						stackPop(stack);									//pop the token we dont need
						if(tokenPtr -> type  == token_leftHandle)
							rule= 11;										//rule 11 <(E)>
					}
				}
				break;
			default:
				fprintf(stderr,"Unexpected token\n");
				break;
			
		}	
	}else{
		fprintf(stderr,"First token to be reduced is not right handle\n");
		
	}		
	
	
	free(tokenPtr);
	free(bufferStack);
	return rule;
}	

void reduction(Token* tokenPtr, Token* stackTopPtr,tStack* stack){
	stackTopPtr=stackTop(stack);
	char whatToDo = precedence_table[stackTopPtr -> type][ tokenPtr -> type];
	printf("input is:    %d\n", tokenPtr->type);					//TODO test-output,delete later
	printf("stack top is %d\n", stackTopPtr->type);
	printf("What to do is: %c\n\n",whatToDo);	
	Token *toBePushed = malloc(sizeof(Token));
	if(whatToDo == '<'){
		toBePushed -> type = token_leftHandle;
		stackPush(stack, toBePushed);
		stackPush(stack, tokenPtr);
	}
	if(whatToDo == '>'){
		toBePushed -> type = token_rightHandle;
		stackPush(stack,toBePushed);
		whatRule(stack);
	}
	if(whatToDo == '='){
		toBePushed -> type = token_leftHandle;	//TODO make sure its ok
	}
	
	
	free(toBePushed);
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

int runPrecedenceAnalysis(FILE* f){	
	tStack* stack=malloc(sizeof(tStack));	//initialize stack
	stack->top=-1;
	Token* tokenPtrTmp = tokenInit();			//init for the first $
	tokenPtrTmp->type=token_dollar;
	stackPush(stack,tokenPtrTmp);
	Token* stackTopPtr=stackTop(stack);	//initialize stack pointer	
	Token* tokenPtr=NULL;	
	for(int i=0; i<=8 ;i++){				//TODO temporary testing loop
		tokenPtr= getToken(f);
		stackTopPtr = stackTop(stack);
		reduction(tokenPtr, stackTopPtr, stack);	
	}
	free(stack);	
	return 0;
}































