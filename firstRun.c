	#include "syntax.h"
extern resourceStruct* resources;

void functionInsert(Token * tokenName, Token * tokenType,Token * tokenClass){
	htabInsertReturnType(resources->functionTable, tokenName->name, tokenClass->name, tokenType->type);
}

void globalVarInsert(Token * tokenName, Token * tokenType,Token * tokenClass){
	//htabInsert(globalVarTable, tokenName -> name);
	htabInsertVarType(tokenName->name, tokenClass->name, tokenType->type);
}

int firstRun(FILE * f){
	Token * tokenPtr = NULL;
	Token * tokenPtr1 = NULL;
	Token * tokenPtr2 = NULL;
	Token * tokenPtr3 = NULL;
	Token * classPtr = NULL;


	while(1){
		tokenPtr3=tokenPtr2;
		tokenPtr2=tokenPtr1;
		tokenPtr1=tokenPtr;
		tokenPtr=getToken(f);
		if(tokenPtr1!=NULL && tokenPtr1->type==token_class && tokenPtr->type==token_identifier){
			classPtr=tokenPtr;
		}
		if(tokenPtr ->type == token_EOF){
			break;
		}
		if(tokenPtr -> type == token_bracketLeftRound ){
			if (tokenPtr1 -> type == token_identifier ){
				if(tokenPtr2!=NULL && (tokenPtr2->type==token_int || tokenPtr2->type==token_double || tokenPtr2->type==token_String || tokenPtr2->type==token_void)){
					if(tokenPtr3!=NULL && tokenPtr3->type == token_static){
						functionInsert(tokenPtr1, tokenPtr2, classPtr);
					}
				}
			}
		}

		if(tokenPtr -> type == token_assign || tokenPtr -> type == token_semicolon ){
			if (tokenPtr1!=NULL && tokenPtr1 -> type == token_identifier ){
				if(tokenPtr2!= NULL && (tokenPtr2->type==token_int || tokenPtr2->type==token_double || tokenPtr2->type==token_String)){
					if(tokenPtr3!=NULL && tokenPtr3->type == token_static){
						//printf("PISU STACICKOU PROMENNOU YO\n\n\n\n\n");
						globalVarInsert(tokenPtr1, tokenPtr2, classPtr);
					}

				}
				if(tokenPtr2!=NULL && tokenPtr3!=NULL && tokenPtr2->type==token_void && tokenPtr3->type==token_static)
				{
					fprintf(stderr, "Syntax error, variable type void\n");
					exit(2);
				}
			}
		}


	}
	//printHtab(resources->functionTable, 0);
	//printHtab(globalVarTable, 1);
	return 0;
}
