#include "interpret.h"
#define INT 28
#define DOUBLE 23
#define STRING 30

char *strtok_r(char *, const char *, char **);	//required for strtok_r function
extern resourceStruct * resources;

int runInterpret(tListOfInstr *list){
	fprintf(stderr,"\n");
	listFirst(list);
	
	while(list->active->nextItem !=NULL){
		printInstType(list->active->Instruction.instType);
		fprintf(stderr,": %s, %s, %s\n", (list->active->Instruction.addr1),(list->active->Instruction.addr2),(list->active->Instruction.addr3));
		listNext(list);
	}
	printInstType(list->active->Instruction.instType);	//print the very last instr
	fprintf(stderr,"\n");
	
	listFirst(list);
	thTable * localVarTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);
	htabInit(localVarTable);
	
	fprintf(stderr,"\n");	//TODO comment me
	interpretEval(list,localVarTable);
		
	memfreeall();
	exit(0);
	
return 0;	
}

thtabItem* interpretEval(tListOfInstr *list, thTable* localVarTable){
	struct listItem *lastActive;
	thTable * nextCallTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);		//Alocate memory for var table, which will be passed to new function
	htabInit(nextCallTable);
	char currentFunc[2047];
	char currentClass[2047];
	char* dot=".";
	thtabItem* itemPtr = NULL;		//pointer to 1st address
	thtabItem* itemPtr2 = NULL;		//pointer to 2nd address
	thtabItem* itemPtr3 = NULL;		//pointer to 3rd address
	int jumpBool=0;					//if !0 then jump is executed
	char* buf;
	
	while(1){
		fprintf(stderr,"interpreting instr: %d, %s, %s, %s\n",list->active->Instruction.instType,list->active->Instruction.addr1, list->active->Instruction.addr2,list->active->Instruction.addr3);
		switch (list->active->Instruction.instType){


	//************************I_STOP******************************//
			case I_STOP:
				
				fprintf(stderr,"\nInterpret over.\n\n");	//TODO comment me
		printHtabLocal(localVarTable);	//TODO delete me		
				memfreeall();
				exit(0);	

			break;
			
	//************************I_LABEL******************************//
			case I_LABEL:	;
				if(strstr(list->active->Instruction.addr1,dot)){
					char buffer[2047];
					char *buffer2;
					strcpy(buffer, list->active->Instruction.addr1);
					strtok_r(buffer, ".", &buffer2);
					strcpy(currentClass,buffer);
					strcpy(currentFunc,buffer2);
				}

			break;
			
	//************************I_GOTO******************************//
			case I_GOTO:
				lastActive=list->active;	//save pointer to active instr
				listFirst(list);			//reset active to first for label search
				while(1){				//search through instructions and stop when label with same name as searched for is found
					if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr1)==0){
						break;
					}	
					if(list->active->nextItem == NULL){		//the label was not found, This is likely error in OUR code.
						fprintf(stderr,"\ncalled function not found\n\n\n"); 
						memfreeall();
						exit(3);			
					}
					listNext(list);
				}
			break;
			
	//************************I_NEW_VAR******************************//
			case I_NEW_VAR:
				htabInsert(localVarTable, list->active->Instruction.addr1, atoi(list->active->Instruction.addr2));
			break;
			
	//************************I_FN_END******************************//
			case I_FN_END:
				buf=concat(currentFunc,currentClass);
				itemPtr=htabSearch(resources->functionTable,buf);
				if(itemPtr->returnType != 33){			//return type is not void, but no return was found
					fprintf(stderr,"I_FN_END. Function has ended but no return found.\n");
					memfreeall();
					exit(8);
				}
		printHtabLocal(localVarTable)		;
				return NULL;
			break;
			
	//************************I_RETURN******************************//
			case I_RETURN:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_RETURN. Returning nonexistant variable.\n");
						memfreeall();
						exit(3);
					}
				}
				thtabItem *returnPtr = memalloc(sizeof(thtabItem));
				returnPtr = memcpy(returnPtr,itemPtr,sizeof(thtabItem));
				
				return returnPtr;
			break;
			
	//************************I_IF_GOTO******************************//
			case I_IF_GOTO:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_IF_GOTO expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}	
				if(itemPtr->varType == 28 && itemPtr->intValue == 0)jumpBool=1;		//logical switch is INT
				else if(itemPtr->varType == 23 && itemPtr->doubleValue == 0)jumpBool=1;		//logical switch is BOOL
				else {jumpBool=0;}
				
				if(jumpBool){	
					lastActive=list->active;	//save pointer to active instr
					listFirst(list);			//reset active to first for label search
					while(1){			//search through instructions until the same label is found
						if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr2)==0){					
							break;
						}	
						if(list->active->nextItem == NULL){		//if label was not found
							fprintf(stderr,"\ncalled function not found\n\n\n"); 
							memfreeall();			//user has called a function that does not exist.
							exit(3);		
						}
						listNext(list);
					}				
				}

				jumpBool=0;

			break;
			
	//************************I_FN_CALL******************************//
			case I_FN_CALL:
				if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
				}	
				lastActive=list->active;	//save pointer to active instr
				listFirst(list);			//reset active to first for label search
				while(1){			//search through instructions until the same label is found
					if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr1)==0){
						break;
					}	
					if(list->active->nextItem == NULL){		//if label was not found
						fprintf(stderr,"\ncalled function not found\n\n\n"); 
						memfreeall();			//user has called a function that does not exist.
						exit(3);		
					}
					listNext(list);
				}
				interpretEval(list,nextCallTable);		//recursive call this function
				htabDispose(nextCallTable);				//after return we dispose the variable table from called function
				htabInit(nextCallTable);
				list->active=lastActive;	//restore active position before fn call
			break;		
	
	//************************I_PROGRAM******************************//
			case I_PROGRAM:
				strcpy(currentClass, "Main");
				strcpy(currentFunc, "run");
				listNext(list);
			break;
			
	//************************I_MOV_INT******************************//
			case I_MOV_INT:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = INT;
					itemPtr->intValue = atoi(list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_INT to nonexistant variable.\n");
					exit(3);
				}
			break;		
			
	//************************I_MOV_DOUBLE******************************//
			case I_MOV_DOUBLE:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = DOUBLE;
					itemPtr->doubleValue = atof(list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_DOUBLE to nonexistant variable.\n");
					exit(3);
				}
			break;	
			
	//************************I_MOV_STRING******************************//
			case I_MOV_STRING:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = STRING;
					itemPtr->stringValue=memalloc(sizeof(list->active->Instruction.addr2));
					strcpy(itemPtr->stringValue,list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_STRING to nonexistant variable.\n");
					exit(3);
				}
			break;	

	//************************I_ADD******************************//
			case I_ADD:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_SUB nonexistant target variable.\n");
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					printHtabLocal(localVarTable);	//Variable is not in var table exist
					printHtab(resources->globalVarTable,1);
					fprintf(stderr,"Sem_Error. I_SUB nonexistant left operand variable.\n");
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_SUB nonexistant right operand variable.\n");
						exit(3);
					}
				}
				
				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28 && itemPtr->varType != 30){
					fprintf(stderr, "I_SUB target not INT or DOUBLE \n");
					memfreeall();
					exit(3);
				}
				
				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}
				
				
				if(itemPtr2->varType == 28){			// INT + 
					if(itemPtr3->varType == 28){		// INT + INT	
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=28;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->intValue=itemPtr2->intValue+itemPtr3->intValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 23){	// INT + D
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->intValue=itemPtr->intValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 30){	// INT + STR
						if(itemPtr->varType == 23){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 28){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 30 || itemPtr->varType == 0){
							itemPtr->stringValue=memalloc(sizeof(itemPtr2->intValue)+sizeof(itemPtr3->stringValue));
							sprintf(itemPtr->stringValue,"%d%s",itemPtr2->intValue, itemPtr3->stringValue);
							itemPtr->isInit=1;
							itemPtr->varType=30;
						}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->intValue=itemPtr2->intValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;
					}else{
						fprintf(stderr, "I_ADD right operand not INT or DOUBLE \n");
						memfreeall();
						exit(4);
					}
						
						
	
				}else if(itemPtr2->varType == 23){		// DOUBLE +
					if(itemPtr3->varType == 28){		// DOUBLE + INT
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->doubleValue=itemPtr2->doubleValue+itemPtr3->intValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 23){	// DOUBLE + DOUBLE
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->doubleValue=itemPtr2->doubleValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;
					
					}else if(itemPtr3->varType == 30){	// DOUBLE + STR
						if(itemPtr->varType == 23){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 28){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 30 || itemPtr->varType == 0){
							itemPtr->stringValue=memalloc(sizeof(itemPtr2->doubleValue)+sizeof(itemPtr3->stringValue));
							sprintf(itemPtr->stringValue,"%g%s",itemPtr2->doubleValue, itemPtr3->stringValue);
							itemPtr->isInit=1;
							itemPtr->varType=30;
						}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->intValue=itemPtr2->intValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;
					}else{
						fprintf(stderr, "I_ADD right operand not INT or DOUBLE \n");
						memfreeall();
						exit(3);
					}	
						
							
				}else if(itemPtr2->varType == 30){					// STRING + 
					if(itemPtr3->varType == 28){					// STRING + INT
						if(itemPtr->varType == 23 || itemPtr->varType == 28){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 0 || itemPtr->varType == 30 ){ itemPtr->varType = 30;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->stringValue=memalloc(sizeof(itemPtr2->stringValue)+sizeof(itemPtr3->intValue));
						sprintf(itemPtr->stringValue,"%s%d",itemPtr2->stringValue, itemPtr3->intValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else if(itemPtr3->varType == 23){				// STRING + DOUBLE
						if(itemPtr->varType == 23 || itemPtr->varType == 28){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 0 || itemPtr->varType == 30 ){ itemPtr->varType = 30;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->stringValue=memalloc(sizeof(itemPtr2->stringValue)+sizeof(itemPtr3->doubleValue));
						sprintf(itemPtr->stringValue,"%s%g",itemPtr2->stringValue, itemPtr3->doubleValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else if(itemPtr3->varType == 30){				// STRING + STRING
						if(itemPtr->varType == 23 || itemPtr->varType == 28){ 
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						else if(itemPtr->varType == 0 || itemPtr->varType == 30 ){ itemPtr->varType = 30;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->stringValue=memalloc(sizeof(itemPtr2->stringValue)+sizeof(itemPtr3->stringValue));
						sprintf(itemPtr->stringValue,"%s%s",itemPtr2->stringValue, itemPtr3->stringValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else{
						fprintf(stderr, "I_ADD right operand not INT or DOUBLE \n");
						memfreeall();
						exit(3);
					}		
							
		
				}else{
					fprintf(stderr, "I_ADD target not INT or DOUBLE \n");
					memfreeall();
					exit(3);
				}
				
					
				
			
			break;
	
	//************************I_SUB******************************//
			case I_SUB:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_SUB nonexistant target variable.\n");
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					printHtabLocal(localVarTable);	//Variable is not in var table exist
					printHtab(resources->globalVarTable,1);
					fprintf(stderr,"Sem_Error. I_SUB nonexistant left operand variable.\n");
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_SUB nonexistant right operand variable.\n");
						exit(3);
					}
				}
				
				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_SUB target not INT or DOUBLE \n");
					memfreeall();
					exit(3);
				}
				
				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}
				
				if(itemPtr2->varType == 28){	//left operand is int	INT-
					if(itemPtr3->varType == 28){	//and right operand is also int	INT-INT
						if(itemPtr->varType == 0 ){		//tmp var not initialized
							itemPtr->varType = 28;			//new var will be int
						}
						itemPtr->intValue = itemPtr2->intValue - itemPtr3->intValue;
						itemPtr->isInit = 1;
					}else{
						if(itemPtr3->varType == 23){		//INT - double 
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}
							if(itemPtr->varType == 23 || itemPtr->varType == 28){
								itemPtr->varType = 23;		//target will be double
								itemPtr->doubleValue = itemPtr2->intValue - itemPtr3->doubleValue;
								itemPtr->isInit = 1;
							}else{
								fprintf(stderr, "I_SUB target not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}
					
				}else{
					if (itemPtr2->varType == 23){	//left operand is double	
						if(itemPtr3->varType == 28){		// double - int
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}	
							itemPtr->varType = 23;		//target will be double
							itemPtr->doubleValue = itemPtr2->doubleValue - itemPtr3->intValue;
							itemPtr->isInit = 1;
						}else{
							if(itemPtr3->varType == 23){		//double - double
								if(itemPtr->varType == 0 ){		//tmp var not initialized
									itemPtr->varType = 23;			//new var will be double
								}	
								itemPtr->varType = 23;		//target will be double
								itemPtr->doubleValue = itemPtr2->doubleValue - itemPtr3->doubleValue;
								itemPtr->isInit = 1;
									
							}else{								//double - X
								fprintf(stderr, "I_SUB right operand not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}else{
						fprintf(stderr, "I_SUB left operand not INT or DOUBLE \n");
						memfreeall();
						exit(3);
					}
				} 	
			break;	
			
	//************************I_MUL******************************//
			case I_MUL:		//first adress search
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_MUL nonexistant target variable.\n");
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					printHtabLocal(localVarTable);	//Variable is not in var table exist
					printHtab(resources->globalVarTable,1);
					fprintf(stderr,"Sem_Error. I_MUL nonexistant left operand variable.\n");
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_MUL nonexistant right operand variable.\n");
						exit(3);
					}
				}
				
				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_MUL target not INT or DOUBLE \n");
					memfreeall();
					exit(3);
				}
				
				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}
				
				if(itemPtr2->varType == 28){	//left operand is int	INT*
					if(itemPtr3->varType == 28){	//and right operand is also int	INT*INT
						if(itemPtr->varType == 0 ){		//tmp var not initialized
							itemPtr->varType = 28;			//new var will be int
						}
						itemPtr->intValue = itemPtr2->intValue * itemPtr3->intValue;
						itemPtr->isInit = 1;
					}else{
						if(itemPtr3->varType == 23){		//INT * double 
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}
							if(itemPtr->varType == 23 || itemPtr->varType == 28){
								itemPtr->varType = 23;		//target will be double
								itemPtr->doubleValue = itemPtr2->intValue * itemPtr3->doubleValue;
								itemPtr->isInit = 1;
							}else{
								fprintf(stderr, "I_MUL target not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}
					
				}else{
					if (itemPtr2->varType == 23){	//left operand is double	
						if(itemPtr3->varType == 28){		// double * int
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}	
							itemPtr->varType = 23;		//target will be double
							itemPtr->doubleValue = itemPtr2->doubleValue * itemPtr3->intValue;
							itemPtr->isInit = 1;
						}else{
							if(itemPtr3->varType == 23){		//double * double
								if(itemPtr->varType == 0 ){		//tmp var not initialized
									itemPtr->varType = 23;			//new var will be double
								}	
								itemPtr->varType = 23;		//target will be double
								itemPtr->doubleValue = itemPtr2->doubleValue * itemPtr3->doubleValue;
								itemPtr->isInit = 1;
									
							}else{								//double * X
								fprintf(stderr, "I_MUL right operand not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}else{
						fprintf(stderr, "I_MUL left operand not INT or DOUBLE \n");
						memfreeall();
						exit(3);
					}
				} 	
				
			break;	
			
	//************************I_DIV******************************//
			case I_DIV: //adress search
			
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_DIV nonexistant target variable.\n");
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					printHtabLocal(localVarTable);	//Variable is not in var table exist
					printHtab(resources->globalVarTable,1);
					fprintf(stderr,"Sem_Error. I_DIV nonexistant left operand variable.\n");
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_DIV nonexistant right operand variable.\n");
						exit(3);
					}
				}

				
				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_DIV target not INT or DOUBLE \n");
					memfreeall();
					exit(3);
				}
				
				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}
				
				if(itemPtr2->varType == 28){	//left operand is int	INT/
					if(itemPtr3->varType == 28){	//and right operand is also int	INT/INT
						if(itemPtr->varType == 0 ){		//tmp var not initialized
							itemPtr->varType = 28;			//new var will be int
						}
						if(itemPtr3->intValue == 0){
									fprintf(stderr,"Division by zero.\n");
									memfreeall();
									exit(9);
						}
						itemPtr->intValue = itemPtr2->intValue / itemPtr3->intValue;
						itemPtr->isInit = 1;
					}else{
						if(itemPtr3->varType == 23){		//INT / double 
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}
							if(itemPtr->varType == 23 || itemPtr->varType == 28){
								itemPtr->varType = 23;		//target will be double
								if(itemPtr3->doubleValue == 0){
									fprintf(stderr,"Division by zero.\n");
									memfreeall();
									exit(9);
								}
								itemPtr->doubleValue = itemPtr2->intValue / itemPtr3->doubleValue;
								itemPtr->isInit = 1;
							}else{
								fprintf(stderr, "I_DIV target not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}
					
				}else{
					if (itemPtr2->varType == 23){	//left operand is double	
						if(itemPtr3->varType == 28){		// double / int
							if(itemPtr->varType == 0 ){		//tmp var not initialized
								itemPtr->varType = 23;			//new var will be double
							}	
							itemPtr->varType = 23;		//target will be double
							if(itemPtr3->intValue == 0){
									fprintf(stderr,"Division by zero.\n");
									memfreeall();
									exit(9);
							}
							itemPtr->doubleValue = itemPtr2->doubleValue / itemPtr3->intValue;
							itemPtr->isInit = 1;
						}else{
							if(itemPtr3->varType == 23){		//double / double
								if(itemPtr->varType == 0 ){		//tmp var not initialized
									itemPtr->varType = 23;			//new var will be double
								}	
								itemPtr->varType = 23;		//target will be double
								if(itemPtr3->doubleValue == 0){
									fprintf(stderr,"Division by zero.\n");
									memfreeall();
									exit(9);
								}
								itemPtr->doubleValue = itemPtr2->doubleValue / itemPtr3->doubleValue;
								itemPtr->isInit = 1;
									
							}else{								//double / X
								fprintf(stderr, "I_DIV right operand not INT or DOUBLE \n");
								memfreeall();
								exit(3);
							}
						}
					}else{
						fprintf(stderr, "I_DIV left operand not INT or DOUBLE \n");
						memfreeall();
						exit(3);
					}
				}

			break;	
			
	//************************I_LT******************************//
			case I_LT:

			break;				

	//************************I_GT******************************//
			case I_GT:

			break;		
			
	//************************I_LE******************************//
			case I_LE:

			break;	

	//************************I_GE******************************//
			case I_GE:

			break;				

	//************************I_EQ******************************//
			case I_EQ:

			break;	

	//************************I_NE******************************//
			case I_NE:

			break;	

	//************************I_MOV******************************//
			case I_MOV:
			
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_MOV to nonexistant target variable.\n");
						exit(3);
					}
				}
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_MOV to nonexistant source variable.\n");
						exit(3);
					}
				}
				
				if(itemPtr->varType != itemPtr2->varType){		//Types are not matching, error4
					if((itemPtr->varType != 23 && itemPtr->varType != 28) ||  (itemPtr2->varType != 23 && itemPtr2->varType != 28)){		
						printHtabLocal(localVarTable);
						fprintf(stderr,"I_MOV source and target type not matching.\n");
						exit(4);
					}
				}
				if(itemPtr2->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}
				
				itemPtr->isInit=1;					//Mark the variable as initialized. It can be now used in expressions.
				switch (itemPtr2->varType){
					case 28:			//source is type int
						if(itemPtr->varType == 23){
							itemPtr->doubleValue=itemPtr2->intValue;
						}else{
							itemPtr->intValue=itemPtr2->intValue;	
						}
						break;
					case 30:			//source is type String		
						itemPtr->stringValue = memalloc(4*sizeof(itemPtr2->stringValue));	
						strcpy(itemPtr->stringValue,itemPtr2->stringValue);
						itemPtr->varType=itemPtr2->varType;
						break;
					case 23:			//source is type double
						if(itemPtr->varType == 28){
							itemPtr->varType=23;
						}
						itemPtr->doubleValue=itemPtr2->doubleValue;						
						break;
						
					default:
						fprintf(stderr,"I_MOV source operand has unexpected type %d\n",itemPtr->varType);
						exit(3);	//TODO this is our fault, exit code is hard to define....
				}
			break;	


	//************************I_WHILE_GOTO******************************//
			case I_WHILE_GOTO:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						printHtabLocal(localVarTable);	//Variable is not in var table exist
						printHtab(resources->globalVarTable,1);
						fprintf(stderr,"Sem_Error. I_WHILE_GOTO expression based on nonexistant variable\n");
						exit(3);
					}
				}	
				if(itemPtr->varType == 28 && itemPtr->intValue == 0)jumpBool=1;		//logical switch is INT
				else if(itemPtr->varType == 23 && itemPtr->doubleValue == 0)jumpBool=1;		//logical switch is BOOL
				else {jumpBool=0;}
				
				if(jumpBool){	
					lastActive=list->active;	//save pointer to active instr
					listFirst(list);			//reset active to first for label search
					while(1){			//search through instructions until the same label is found
						if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr2)==0){
							
							break;
						}	
						if(list->active->nextItem == NULL){		//if label was not found
							fprintf(stderr,"\ncalled function not found\n\n\n"); 
							memfreeall();			//user has called a function that does not exist.
							exit(3);		
						}
						listNext(list);
					}				
				}	
				
				jumpBool=0;
			break;		

	//************************I_RETURN_NOTHING******************************//
			case I_RETURN_NOTHING:

			break;				

	//************************I_PUSH******************************//
			case I_PUSH:

			break;
			
	//************************I_CLEAR_TMPS******************************//
			case I_CLEAR_TMPS:
				htabDeleteHashtag(localVarTable);
			break;		

	//************************I_RETURN_MOV******************************//
			case I_RETURN_MOV:

			break;			
			
			
			default:
				fprintf(stderr,"Interpreting an unknown instruction %d\n",list->active->Instruction.instType);
			return NULL;
		}
		listNext(list);
		
		
	}
}


void printInstType(int instructionType){
	switch (instructionType){
		case I_STOP:		fprintf(stderr,"  I_STOP:\t"); 	break;
		case I_LABEL:		fprintf(stderr,"I_LABEL:\t"); 		break;
		case I_GOTO:		fprintf(stderr,"  I_GOTO:\t"); 		break;
		case I_NEW_VAR:		fprintf(stderr,"  I_NEW_VAR:\t"); 	break;
		case I_FN_END:		fprintf(stderr,"  I_FN_END:\t"); 	break;
		case I_RETURN:		fprintf(stderr,"  I_RETURN:\t"); 	break;
		case I_IF_GOTO:		fprintf(stderr,"  I_IF_GOTO:\t"); 	break;
		case I_FN_CALL:		fprintf(stderr,"  I_FN_CALL:\t"); 	break;
		case I_PROGRAM:		fprintf(stderr,"  I_PROGRAM:\t"); 	break;
		case I_MOV_INT:		fprintf(stderr,"  I_MOV_INT:\t"); 	break;
		case I_MOV_DOUBLE:	fprintf(stderr,"  I_MOV_DOUBLE:\t"); 	break;
		case I_MOV_STRING:	fprintf(stderr,"  I_MOV_STRING:\t"); 	break;
		case I_ADD:		fprintf(stderr,"  I_ADD:\t"); 	break;
		case I_SUB:		fprintf(stderr,"  I_SUB:\t"); 	break;
		case I_MUL:		fprintf(stderr,"  I_MUL:\t"); 	break;
		case I_DIV:		fprintf(stderr,"  I_DIV:\t"); 	break;
		case I_LT:		fprintf(stderr,"  I_LT:\t\t"); 	break;
		case I_GT:		fprintf(stderr,"  I_GT:\t\t"); 	break;
		case I_LE:		fprintf(stderr,"  I_LE:\t\t"); 	break;
		case I_GE:		fprintf(stderr,"  I_GE:\t\t"); 	break;
		case I_EQ:		fprintf(stderr,"  I_EQ:\t\t"); 	break;
		case I_NE:		fprintf(stderr,"  I_NE:\t\t"); 	break;
		case I_MOV:		fprintf(stderr,"  I_MOV:\t"); 	break;
		case I_WHILE_GOTO:		fprintf(stderr,"  I_WHILE_GOTO:\t"); 	break;
		case I_RETURN_NOTHING:		fprintf(stderr,"  I_RETURN_NOTHING:\t"); 	break;
		case I_PUSH:		fprintf(stderr,"  I_PUSH:\t"); 	break;
		case I_CLEAR_TMPS:		fprintf(stderr,"  I_CLEAR_TMPS:\t"); 	break;
		case I_RETURN_MOV:		fprintf(stderr,"  I_RETURN_MOV:\t"); 	break;
		default: fprintf(stderr," unknown instruciton found:\t"); 	break;
	}
}