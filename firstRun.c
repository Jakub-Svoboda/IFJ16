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
						char *dot=".";
						if(strstr(tokenPtr1->name,dot)){
							fprintf(stderr,"Fully qualified identifier in declaration\n");
							memfreeall();
							exit(2);				//TODO check if right exit value
						}
						else{
							globalVarInsert(tokenPtr1, tokenPtr2, classPtr);
						}
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
	thtabItem* ptr =htabSearch(resources->functionTable,"Main.run");
	char *key="Main.run";						//initialize variable for Main.run existence check
			if(htabSearch(resources->functionTable, key)==NULL){	//if function run in class Main does not exist
					fprintf(stderr, "Cannot find funtion \"run\" in class \"Main\"\n");		//print error and exit
					exit(3);					//TODO call garbage collector
			}			
		if(ptr->returnType!=token_void){
			memfreeall();
			exit(3);
		}
	//printHtab(resources->functionTable, 0);
	//printHtab(globalVarTable, 1);
	return 0;
}
