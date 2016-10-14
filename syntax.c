#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
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

void stackPush(tStack* s,char inputChar){
	s->top=s->top+1;
	s->arr[s->top]=inputChar;	
}

void stackPop(tStack* s){
	if(stackEmpty(s)){													//check for empty stack
		printf("StackPop requested but stack is empty.\n");
	}else{
		s->arr[s->top] ='0';
		(s->top)--;
	}	
}

void stackTop(tStack* s, char* c){
	if(stackEmpty(s)){													//check for empty stack
		printf("StackTop requested but stack is empty.\n");
	}else{
		*c=s->arr[s->top];
	}	
}

int stackEmpty (tStack* s){
	return(MAX_STACK-1 == s->top);
}

int runPrecedenceAnalysis(){
	char vysledek[100]  ="";
	char simulatedStack[100] ="";
	char input[100] ="";
	char inputChar[100];
	
	tStack *stack=malloc(sizeof(tStack));	//initialize stack
	stack->top=-1;
	
	
	while(1) {
		printf("what is input?\n");
		scanf("%s", input);
		printf("what is our stack?\n");
		scanf("%s", simulatedStack);
		printf("what is vysledek?\n");
		scanf("%s", vysledek);
	
	}
	
	
	free(stack);			
	return 0;
}
