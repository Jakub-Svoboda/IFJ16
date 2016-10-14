#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
#include "scanner.h"


char precedence_table[14][14]={					//TODO Why is the range 14 though? 
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
{'<','<','<','<','<','<','<','<','<','<','<','=','<','e'},	//(
{'>','>','>','>','>','>','>','>','>','>','e','>','e','>'},	//)
{'>','>','>','>','>','>','>','>','>','>','e','>','e','>'},	//i
{'<','<','<','<','<','<','<','<','<','<','<','e','<','e'},	//$       	stack
};

//void reduction(char *c){
//	char reductionn;
//	switch (c){
//		case "<E+E>":
//			reduction = "E"	;
//			break;
//	}
// }


void stackPush(tStack *s,struct Token *Token){
	s->top=s->top+1;
	s->arr[s->top]=Token;	
}

void stackPop(tStack *s){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackPop requested but stack is empty.\n");
	}else{
		s->arr[s->top] = NULL ;
		(s->top)--;
	}	
}

void stackTop(tStack *s, struct Token *Token){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackTop requested but stack is empty.\n");
	}else{
		Token=s->arr[s->top];
	}	
}

int stackEmpty (tStack *s){
	return(MAX_STACK-1 == s->top);
}

int runPrecedenceAnalysis(FILE *f){
//	char vysledek[100]  ="";
//	char simulatedStack[100] ="";
//	char input[100] ="";
//	char inputChar[100];
	
	tStack *stack=malloc(sizeof(tStack));	//initialize stack
	stack->top=-1;
	Token *tokenPtr = getToken(f);
	tokenPtr=tokenPtr;					//TODO deleteme
	
	
	
//	while(1) {
//		printf("what is input?\n");
//		scanf("%s", input);
//		printf("what is our stack?\n");
//		scanf("%s", simulatedStack);
//		printf("what is vysledek?\n");
//		scanf("%s", vysledek);
	
//	}
	
	
	free(stack);			
	return 0;
}
