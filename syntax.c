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

//void reduction(char *c){
//	char reductionn;
//	switch (c){
//		case "<E+E>":
//			reduction = "E"	;
//			break;
//	}
// }


void stackPush(tStack* s,Token* Token){
	s->top=s->top+1;
	s->arr[s->top]=Token;	
	printf("A token has been pushed: ");		//TODO test-output,delete later
	printf("%d\n",Token->type);					//TODO test-output,delete later
}

void stackPop(tStack* s){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackPop requested but stack is empty.\n");
	}else{
		s->arr[s->top] = NULL ;
		(s->top)--;
	}	
}

void stackTop(tStack* s,Token* Token){
	if(stackEmpty(s)){													//check for empty stack
		fprintf(stderr,"StackTop requested but stack is empty.\n");
	}else{
		Token=s->arr[s->top];
	}	
	Token = Token;		//TODO check wtf is wrong with this. Something causes warning without this.
}

int stackEmpty (tStack* s){
	return(MAX_STACK-1 == s->top);
}

int runPrecedenceAnalysis(FILE* f){
	
	tStack* stack=malloc(sizeof(tStack));	//initialize stack
	stack->top=-1;

	Token* tokenPtr = tokenInit();			//init for the first $
	tokenPtr->type=token_dollar;
	stackPush(stack,tokenPtr);
	free(tokenPtr);
	
	for(int i=0; i<=8 ;i++){				//TODO temporary testing loop
		 tokenPtr= getToken(f);
		 stackPush(stack,tokenPtr);
		 free(tokenPtr);
	}
	
	
	free(stack);
		
	return 0;
}
