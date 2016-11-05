#include "firstRun.h"

void functionInsert(Token * tokenPtr, thTable * functionTable){
	htabInsert(functionTable, tokenPtr->name);
}

void globalVarInsert(Token * tokenPtr, thTable * globalVarTable){
	htabInsert(globalVarTable, tokenPtr -> name);	
}

int firstRun(thTable * functionTable, thTable * globalVarTable,FILE * f){
	Token * tokenPtr = NULL;
	Token * tokenPtr1 = NULL;
	Token * tokenPtr2 = NULL;
	Token * tokenPtr3 = NULL;
	
	while(1){
		tokenPtr3=tokenPtr2;
		tokenPtr2=tokenPtr1;
		tokenPtr1=tokenPtr;
		tokenPtr=getToken(f);
		if(tokenPtr ->type == token_EOF){
			break;
		}
		if(tokenPtr -> type == token_bracketLeftRound ){
			if (tokenPtr1 -> type == token_identifier ){
				if(tokenPtr2->type==token_int || tokenPtr2->type==token_double || tokenPtr2->type==token_String || tokenPtr2->type==token_void){
					if(tokenPtr3->type == token_static){
						printf("PISU FUNKCI YO\n\n\n\n\n");
						functionInsert(tokenPtr1,functionTable);
					}
				}
			}
		}
		
		if(tokenPtr -> type == token_assign || tokenPtr -> type == token_semicolon ){
			if (tokenPtr1 -> type == token_identifier ){
				if(tokenPtr2->type==token_int || tokenPtr2->type==token_double || tokenPtr2->type==token_String || tokenPtr2->type==token_void){
					if(tokenPtr3->type == token_static){
						printf("PISU STACICKOU PROMENNOU YO\n\n\n\n\n");
						globalVarInsert(tokenPtr1,globalVarTable);
					}
				
				}
			}
		}
		
	}
	return 0;
}



