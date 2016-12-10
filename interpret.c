/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    interpret.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "interpret.h"
#define INT 28
#define DOUBLE 23
#define STRING 30

char *strtok_r(char *, const char *, char **);	//required for strtok_r function
extern resourceStruct * resources;

/* This function is called from main. Generates memory for local variable table and initializes it. If there is no error in interpreting, the function exits with 0*/
int runInterpret(tListOfInstr *list){
	listFirst(list);

//	while(list->active->nextItem !=NULL){											//test output for instruction list
//		printInstType(list->active->Instruction.instType);
//		fprintf(stderr,": %s, %s, %s\n", (list->active->Instruction.addr1),(list->active->Instruction.addr2),(list->active->Instruction.addr3));
//		listNext(list);
//	}
//	printInstType(list->active->Instruction.instType);	//print the very last instr
//	fprintf(stderr,"\n");

	listFirst(list);					//resets instruction pointer
	thTable * localVarTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);	//allocates memory for variable table
	htabInit(localVarTable);

	interpretEval(list,localVarTable);											//calls the recursive interpret function

	memfreeall();
	exit(0);


return 0;
}

thtabItem* interpretEval(tListOfInstr *list, thTable* localVarTable){
	struct listItem *lastActive;	//pointer to reset position on instruction lost
	thTable * nextCallTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);		//Alocate memory for var table, which will be passed to new function
	htabInit(nextCallTable);		//initialize new variable table for future function call
	char currentFunc[2047];			//name of current function
	char currentClass[2047];		//name of current class
	char* dot=".";					//char to connect short identifiers
	thtabItem* itemPtr = NULL;		//pointer to 1st address
	thtabItem* itemPtr2 = NULL;		//pointer to 2nd address
	thtabItem* itemPtr3 = NULL;		//pointer to 3rd address
	int jumpBool=0;					//if !0 then jump is executed
	char* buf;
	char postBuffer[2047];
	thtabItem *returnPtr;	//Table item with returned variable
	returnPtr=NULL;
	int argumentCounter = 0;		//counter for argumens 
	int postCounter=1;

	while(1){		//cycle repeats until the end of main function is reached
		//fprintf(stderr,"interpreting instr: %d, %s, %s, %s\n",list->active->Instruction.instType,list->active->Instruction.addr1, list->active->Instruction.addr2,list->active->Instruction.addr3);
		switch (list->active->Instruction.instType){


	//************************I_STOP******************************//
			case I_STOP:			//The last instruction. This should not be executed.
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
					strcpy(currentClass,buffer);			//if a label is function label, mark current class name
					strcpy(currentFunc,buffer2);			//if a label is function label, mark current function name
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
						fprintf(stderr,"\nGOTO label not found %s\n\n\n",list->active->Instruction.addr1);
						memfreeall();
						exit(3);
					}
					listNext(list);
				}
			break;

	//************************I_NEW_VAR******************************//
			case I_NEW_VAR: ;
			char buffer[2047];
			sprintf(buffer, "%s%s%s",currentClass,".",list->active->Instruction.addr1);
				if(htabSearch(resources->functionTable, buffer)==NULL){
				htabInsert(localVarTable, list->active->Instruction.addr1, atoi(list->active->Instruction.addr2));
				}
				else{	
					fprintf(stderr,"Semantic error:Variable name same as function name\n");
					exit(3);
				}
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
				return NULL;
			break;

	//************************I_RETURN******************************//
			case I_RETURN:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {						//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){													//if short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){		//if not in local, search global
						fprintf(stderr,"Sem_Error. I_RETURN. Returning nonexistant variable.\n");
						memfreeall();
						exit(3);
					}
				}
				thtabItem* tmpPtr = htabSearch(resources ->functionTable,concat(currentFunc,currentClass));
				if(itemPtr->varType != tmpPtr->returnType){																//check if returned variable has correct type
					fprintf(stderr,"Sem_Error. I_RETURN. Returning variable type does not match with defined return type.\n");
					memfreeall();
					exit(4);
				}
				if(itemPtr->isInit==0){																					//check for unitilized variable
					fprintf(stderr,"Sem_Error. I_RETURN. Returning variable is not initialized.\n");
					memfreeall();
					exit(8);
				}
				returnPtr = itemPtr;
				return returnPtr;
			break;

	//************************I_IF_GOTO******************************//
			case I_IF_GOTO:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {						//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){													//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){		//if not in local, search global
						fprintf(stderr,"Sem_Error. I_IF_GOTO expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}
				if(itemPtr->varType == 28 && itemPtr->intValue == 0)jumpBool=1;											//logical switch is INT
				else if(itemPtr->varType == 23 && itemPtr->doubleValue == 0)jumpBool=1;									//logical switch is double
				else if(itemPtr->varType == 18 && itemPtr->boolValue == 0)jumpBool=1;									//logical switch is BOOL
				else {jumpBool=0;}

				if(jumpBool){
					lastActive=list->active;																			//save pointer to active instr
					listFirst(list);																					//reset active to first for label search
					while(1){																	//search through instructions until the same label is found
						if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr2)==0){
							break;
						}
						if(list->active->nextItem == NULL){										//if label was not found
							fprintf(stderr,"\ncalled function not found\n\n\n");
							memfreeall();														//user has called a function that does not exist.
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
				lastActive=list->active;								//save pointer to active instr
				listFirst(list);										//reset active to first for label search
				while(1){												//search through instructions until the same label is found
					if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr1)==0){
						break;
					}
					if(list->active->nextItem == NULL){					//if label was not found
						fprintf(stderr,"I_FN_CALL. Called function not found.\n");
						memfreeall();									//user has called a function that does not exist.
						exit(3);
					}
					listNext(list);
				}
				returnPtr=interpretEval(list,nextCallTable);			//recursive call this function
				htabDispose(nextCallTable);								//after return we dispose the variable table from called function
				htabInit(nextCallTable);
				list->active=lastActive;								//restore active position before fn call
				argumentCounter=0;										//reset before next fn call
			break;

	//************************I_PROGRAM******************************//
			case I_PROGRAM:
				while(1){												//search through instructions until the same label is found
					if(list->active->Instruction.instType == 28 && strcmp(list->active->Instruction.addr1, "#PRE0")==0){
						break;
					}
					if(list->active->Instruction.instType == 30) {
						strcpy(currentClass, list->active->Instruction.addr1);
					}
					if(list->active->nextItem == NULL){					//no #PRE0 wrapper was found, starting in Main.run
						strcpy(currentClass, "Main");
						strcpy(currentFunc, "run");
						listFirst(list);
						while(1){										//search through instructions until the same label is found
							if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, "Main.run")==0){
								break;
							}
							listNext(list);
						}
						break;
					}
					listNext(list);
				}


			break;

	//************************I_MOV_INT******************************//
			case I_MOV_INT:															//moves integer from new temprary variable to expression for precedence analysis
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = INT;
					itemPtr->intValue = atoi(list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_INT to nonexistant variable.\n");
					memfreeall();
					exit(3);
				}
			break;

	//************************I_MOV_DOUBLE******************************//
			case I_MOV_DOUBLE:														//moves double from new temprary variable to expression for precedence analysis
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = DOUBLE;
					itemPtr->doubleValue = atof(list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_DOUBLE to nonexistant variable.\n");
					memfreeall();
					exit(3);
				}
			break;

	//************************I_MOV_STRING******************************//
			case I_MOV_STRING:														//moves String from new temprary variable to expression for precedence analysis
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->varType = STRING;
					itemPtr->stringValue=memalloc(sizeof(list->active->Instruction.addr2));
					strcpy(itemPtr->stringValue,list->active->Instruction.addr2);
					itemPtr->isInit=1;
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_STRING to nonexistant variable.\n");
					memfreeall();
					exit(3);
				}
			break;

	//************************I_ADD******************************//
			case I_ADD:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){								//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_ADD nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}																					//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){									//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_ADD nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_ADD nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28 && itemPtr->varType != 30){
					fprintf(stderr, "I_ADD target not INT or DOUBLE \n");
					memfreeall();
					exit(4);
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}


				if(itemPtr2->varType == 28){												// INT +
					if(itemPtr3->varType == 28){											// INT + INT
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=28;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->intValue=itemPtr2->intValue+itemPtr3->intValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 23){										// INT + D
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->doubleValue=itemPtr2->intValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 30){										// INT + STR
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
							itemPtr->stringValue=memalloc(sizeof(itemPtr2->intValue)+sizeof(char)*(strlen(itemPtr3->stringValue)+1));
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
						fprintf(stderr, "I_ADD right operand not INT, double or string \n");
						memfreeall();
						exit(3);
					}



				}else if(itemPtr2->varType == 23){												// DOUBLE +
					if(itemPtr3->varType == 28){												// DOUBLE + INT
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->doubleValue=itemPtr2->doubleValue+itemPtr3->intValue;
						itemPtr->isInit=1;
					}else if(itemPtr3->varType == 23){												// DOUBLE + DOUBLE
						if(itemPtr->varType == 23){ itemPtr->varType=23;}
						else if(itemPtr->varType == 0 || itemPtr->varType == 28){ itemPtr->varType=23;}
						else{
							fprintf(stderr,"I_ADD target not valid type.\n");
							memfreeall();
							exit(4);
						}
						itemPtr->doubleValue=itemPtr2->doubleValue+itemPtr3->doubleValue;
						itemPtr->isInit=1;

					}else if(itemPtr3->varType == 30){												// DOUBLE + STR
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
							itemPtr->stringValue=memalloc(sizeof(itemPtr2->doubleValue)+sizeof(char)*(strlen(itemPtr3->stringValue)+1));
							sprintf(itemPtr->stringValue,"%lf%s",itemPtr2->doubleValue, itemPtr3->stringValue);
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
						fprintf(stderr, "I_ADD right operand not INT,double or string \n");
						memfreeall();
						exit(4);
					}


				}else if(itemPtr2->varType == 30){										// STRING +
					if(itemPtr3->varType == 28){										// STRING + INT
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
						itemPtr->stringValue=memalloc(sizeof(char)*(strlen(itemPtr2->stringValue)+1)+sizeof(itemPtr3->intValue));
						sprintf(itemPtr->stringValue,"%s%d",itemPtr2->stringValue, itemPtr3->intValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else if(itemPtr3->varType == 23){									// STRING + DOUBLE
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
						itemPtr->stringValue=memalloc(sizeof(char)*(strlen(itemPtr2->stringValue)+1)+sizeof(itemPtr3->doubleValue));
						sprintf(itemPtr->stringValue,"%s%lf",itemPtr2->stringValue, itemPtr3->doubleValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else if(itemPtr3->varType == 30){									// STRING + STRING
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
						itemPtr->stringValue=memalloc(sizeof(char)*(strlen(itemPtr2->stringValue)+1)+sizeof(char)*(strlen(itemPtr3->stringValue)+1));
						sprintf(itemPtr->stringValue,"%s%s",itemPtr2->stringValue, itemPtr3->stringValue);
						itemPtr->isInit=1;
						itemPtr->varType=30;
					}else{
						fprintf(stderr, "I_ADD right operand not INT or DOUBLE \n");
						memfreeall();
						exit(4);
					}


				}else{
					fprintf(stderr, "I_ADD target not INT or DOUBLE \n");
					memfreeall();
					exit(4);
				}




			break;

	//************************I_SUB******************************//
			case I_SUB:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_SUB nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_SUB nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_SUB nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_SUB target not INT or DOUBLE \n");
					memfreeall();
					exit(4);
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
								exit(4);
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
								exit(4);
							}
						}
					}else{
						fprintf(stderr, "I_SUB left operand not INT or DOUBLE \n");
						memfreeall();
						exit(4);
					}
				}
			break;

	//************************I_MUL******************************//
			case I_MUL:		//first adress search
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_MUL nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_MUL nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_MUL nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_MUL target not INT or DOUBLE \n");
					memfreeall();
					exit(4);
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
								exit(4);
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
								exit(4);
							}
						}
					}else{
						fprintf(stderr, "I_MUL left operand not INT or DOUBLE \n");
						memfreeall();
						exit(4);
					}
				}

			break;

	//************************I_DIV******************************//
			case I_DIV: //adress search

				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_DIV nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_DIV nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_DIV nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}


				if(itemPtr->varType != 0 && itemPtr->varType != 23 && itemPtr->varType != 28){
					fprintf(stderr, "I_DIV target not INT or DOUBLE \n");
					memfreeall();
					exit(4);
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
								exit(4);
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
								exit(4);
							}
						}
					}else{
						fprintf(stderr, "I_DIV left operand not INT or DOUBLE \n");
						memfreeall();
						exit(4);
					}
				}

			break;

	//************************I_LT******************************//
			case I_LT:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_LT nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_LT nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_LT nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_LT left operandnot INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);	//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_LT left operandnot INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);	//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT <
					if(itemPtr3->varType == 28){							//INT < INT
						if(itemPtr2->intValue < itemPtr3->intValue){		//if less
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT < DOUBLE
						if(itemPtr2->intValue < itemPtr3->doubleValue){		//if less
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE <
					if(itemPtr3->varType == 28){							//DOUBLE< INT
						if(itemPtr2->doubleValue < itemPtr3->intValue){		//if less
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE < DOUBLE
						if(itemPtr2->doubleValue < itemPtr3->doubleValue){		//if less
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized
			break;

	//************************I_GT******************************//
			case I_GT:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_GT nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_GT nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_GT nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_GT left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_GT left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_GT. Operand not initialized: %s: %d or %s: %d.\n",list->active->Instruction.addr2,itemPtr2->isInit, list->active->Instruction.addr3,itemPtr3->isInit );
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT >
					if(itemPtr3->varType == 28){							//INT > INT
						if(itemPtr2->intValue > itemPtr3->intValue){		//if more
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT > DOUBLE
						if(itemPtr2->intValue > itemPtr3->doubleValue){		//if more
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE >
					if(itemPtr3->varType == 28){							//DOUBLE > INT
						if(itemPtr2->doubleValue > itemPtr3->intValue){		//if more
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE > DOUBLE
						if(itemPtr2->doubleValue > itemPtr3->doubleValue){		//if more
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized

			break;

	//************************I_LE******************************//
			case I_LE:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_LE nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_LE nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_LE nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_LE left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_LEleft operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_LE. Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT <=
					if(itemPtr3->varType == 28){							//INT <= INT
						if(itemPtr2->intValue <= itemPtr3->intValue){		//if <=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT <= DOUBLE
						if(itemPtr2->intValue <= itemPtr3->doubleValue){		//if <=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE <=
					if(itemPtr3->varType == 28){							//DOUBLE <= INT
						if(itemPtr2->doubleValue <= itemPtr3->intValue){		//if <=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE <= DOUBLE
						if(itemPtr2->doubleValue <= itemPtr3->doubleValue){		//if <=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized

			break;

	//************************I_GE******************************//
			case I_GE:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_GE nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_GE nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_GE nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_GE left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_GE left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_GE. Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT >=
					if(itemPtr3->varType == 28){							//INT >= INT
						if(itemPtr2->intValue >= itemPtr3->intValue){		//if >=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT >= DOUBLE
						if(itemPtr2->intValue >= itemPtr3->doubleValue){		//if >=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE >=
					if(itemPtr3->varType == 28){							//DOUBLE >= INT
						if(itemPtr2->doubleValue >= itemPtr3->intValue){		//if >=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE >= DOUBLE
						if(itemPtr2->doubleValue >= itemPtr3->doubleValue){		//if >=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized

			break;

	//************************I_EQ******************************//
			case I_EQ:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_EQ nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_EQ nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_EQ nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_EQ left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_EQ left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_EQ. Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT ==
					if(itemPtr3->varType == 28){							//INT == INT
						if(itemPtr2->intValue == itemPtr3->intValue){		//if ==
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT == DOUBLE
						if(itemPtr2->intValue == itemPtr3->doubleValue){		//if ==
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE ==
					if(itemPtr3->varType == 28){							//DOUBLE == INT
						if(itemPtr2->doubleValue == itemPtr3->intValue){		//if ==
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE == DOUBLE
						if(itemPtr2->doubleValue == itemPtr3->doubleValue){		//if ==
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized

			break;

	//************************I_NE******************************//
			case I_NE:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_NE nonexistant target variable.\n");
						memfreeall();
						exit(3);
					}
				}			//second adress search
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
					fprintf(stderr,"Sem_Error. I_NE nonexistant left operand variable.\n");
					memfreeall();
					exit(3);
					}
				}			//third adress search
				if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
					}
					if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_NE nonexistant right operand variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr2->varType != 28 && itemPtr2->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_NE left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}
				if(itemPtr3->varType != 28 && itemPtr3->varType != 23 ){	//operand must be int, double
					fprintf(stderr, "I_NE left operand not INT, DOUBLE or BOOLEAN\n");
					memfreeall();
					exit(4);		//check error code https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=45670
				}

				if(itemPtr2->isInit == 0 || itemPtr3->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_NE. Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				if(itemPtr2->varType == 28){								//INT !=
					if(itemPtr3->varType == 28){							//INT != INT
						if(itemPtr2->intValue != itemPtr3->intValue){		//if !=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//INT != DOUBLE
						if(itemPtr2->intValue != itemPtr3->doubleValue){		//if !=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}else{														//DOUBLE !=
					if(itemPtr3->varType == 28){							//DOUBLE != INT
						if(itemPtr2->doubleValue != itemPtr3->intValue){		//if !=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}else{													//DOUBLE != DOUBLE
						if(itemPtr2->doubleValue != itemPtr3->doubleValue){		//if !=
							itemPtr->boolValue = 1;							//result is TRUE
						}else{
							itemPtr->boolValue = 0;							//result is FALSE
						}
					}
				}
				itemPtr->varType = 18;										//result will be bool
				itemPtr->isInit = 1;										//result becomes initialized

			break;

	//************************I_MOV******************************//
			case I_MOV:
//fprintf(stderr,"\n\naaaaaa %s %s bbbbbb\n\n",list->active->Instruction.addr1,list->active->Instruction.addr2);
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_MOV to nonexistant target variable. %s\n", list->active->Instruction.addr1);
						memfreeall();
						exit(3);
					}
				}
				if((itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) == NULL) {//localVarTable search
					if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
					}
					if((itemPtr2=(htabSearch(resources->globalVarTable,list->active->Instruction.addr2))) == NULL){//if not in local, search global
						fprintf(stderr,"Sem_Error. I_MOV nonexistant source variable.\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->varType != itemPtr2->varType){		//Types are not matching, error4
					if((itemPtr->varType != 23 && itemPtr->varType != 28) ||  (itemPtr2->varType != 23 && itemPtr2->varType != 28)){
						fprintf(stderr,"I_MOV source and target type not matching.\n");
						memfreeall();
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
						itemPtr->stringValue = memalloc(sizeof(char)*(strlen(itemPtr2->stringValue)+1));
						strcpy(itemPtr->stringValue,itemPtr2->stringValue);
						itemPtr->varType=itemPtr2->varType;
						break;
					case 23:			//source is type double
						if(itemPtr->varType == 28){
							itemPtr->varType=23;
							fprintf(stderr,"I_MOV cannot move double to int.\n");
							memfreeall();
							exit(4);
							break;
						}
						itemPtr->doubleValue=itemPtr2->doubleValue;
						break;
					
						
					default:
						fprintf(stderr,"I_MOV source operand has unexpected type %d\n",itemPtr->varType);
						memfreeall();
						exit(99);	//this is our fault, exit code is hard to define....
				}
			break;


	//************************I_WHILE_GOTO******************************//
			case I_WHILE_GOTO:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_WHILE_GOTO expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}
				if(itemPtr->varType == 28 && itemPtr->intValue == 0)jumpBool=1;		//logical switch is INT
				else if(itemPtr->varType == 23 && itemPtr->doubleValue == 0)jumpBool=1;		//logical switch is double
				else if(itemPtr->varType == 18 && itemPtr->boolValue == 0)jumpBool=1;	//logical switch is bool
				else{jumpBool=0;}

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
			case I_RETURN_NOTHING:;
				thtabItem* returnNothingPtr = htabSearch(resources->functionTable,concat(currentFunc,currentClass));
				if(returnNothingPtr->returnType != 33){			//check if return type is void
					fprintf(stderr,"Sem_Error. I_RETURN_NOTHING. Expression to return expected. Return type is: %d \n",returnNothingPtr->returnType);
					memfreeall();
					exit(4);
				}
				return NULL;
			break;

	//************************I_PUSH******************************//
			case I_PUSH:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_PUSH variable not found\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->isInit == 0){
					fprintf(stderr,"I_PUSH. Argument not initialized. \n");
					memfreeall();
					exit(8);
				}

				if(!strstr(list->active->Instruction.addr2,dot)){		//if called function is short identifier
					strcpy(list->active->Instruction.addr2,concat(list->active->Instruction.addr2,currentClass));	//the concat it with class name
				}

				if((itemPtr2=(htabSearch(resources->functionTable,list->active->Instruction.addr2))) == NULL){	//search function table
						fprintf(stderr,"Sem_Error. I_PUSH function not found\n");
						memfreeall();
						exit(3);
				}

				if(itemPtr2->argumentNumber <= argumentCounter){					//check whether there is not an extra argument push
					fprintf(stderr,"I_PUSH. Too many arguments. %d \n", itemPtr2->argumentNumber );
					memfreeall();
					exit(4);
				}

				if(itemPtr2->arguments[argumentCounter] -> argumentType == (int)itemPtr->varType){			//retyping cos token->type is unsigned for some reason
					argumentCounter++;															//if arguments types match, increment the counter
				}else{
					fprintf(stderr,"I_PUSH. Argument type not matching. %d \n", itemPtr2->argumentNumber );
					memfreeall();
					exit(4);
				}
				//if everything ok, insert argument to next call local table
				htabInsert(nextCallTable, itemPtr2->arguments[argumentCounter -1]->argumentName, itemPtr->varType);
				thtabItem* tempPtr = htabSearch(nextCallTable, itemPtr2->arguments[argumentCounter -1]->argumentName);		//search for newly added variable
				if(tempPtr->varType == 23) tempPtr->doubleValue = itemPtr-> doubleValue;
				if(tempPtr->varType == 28) tempPtr->intValue = itemPtr-> intValue;
				if(tempPtr->varType == 30) {
					tempPtr->stringValue = memalloc(sizeof(char)*(strlen(itemPtr->stringValue)+1));
					strcpy(tempPtr->stringValue,itemPtr->stringValue);
				}
				tempPtr->isInit = 1;	//initialize new variable


			break;

	//************************I_CLEAR_TMPS******************************//
			case I_CLEAR_TMPS:
				htabDeleteHashtag(localVarTable);
			break;

	//************************I_RETURN_MOV******************************//
			case I_RETURN_MOV:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"I_RETURN_MOV Sem_Error. Expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}

				if(itemPtr->varType != returnPtr->varType){		//Types are not matching, error4
					if((itemPtr->varType != 23 && itemPtr->varType != 28) ||  (returnPtr->varType != 23 && returnPtr->varType != 28)){
						//printHtabLocal(localVarTable);
						fprintf(stderr,"I_RETURN_MOV  source and target type not matching.\n");
						memfreeall();
						exit(4);
					}
				}
				if(returnPtr->isInit == 0){	//checks if variables are initialized
					fprintf(stderr,"I_RETURN_MOV Operand not initialized.\n");
					memfreeall();
					exit(8);
				}

				itemPtr->isInit=1;					//Mark the variable as initialized. It can be now used in expressions.
				switch (returnPtr->varType){
					case 28:			//source is type int
						if(itemPtr->varType == 23){
							itemPtr->doubleValue=returnPtr->intValue;
						}else{
							itemPtr->intValue=returnPtr->intValue;
						}
						break;

					case 30:			//source is type String
						itemPtr->stringValue = memalloc(sizeof(char)*(strlen(returnPtr->stringValue)+1));
						strcpy(itemPtr->stringValue,returnPtr->stringValue);
						itemPtr->varType=returnPtr->varType;
						break;
					case 23:			//source is type double
						if(itemPtr->varType == 28){
							itemPtr->varType=23;
						}
						itemPtr->doubleValue=returnPtr->doubleValue;
						break;

					default:
						fprintf(stderr,"I_RETURN_MOV source operand has unexpected type %d\n",itemPtr->varType);
						memfreeall();
						exit(99);	//this is our fault, exit code is hard to define....
				}

			break;

	//************************I_GLOBAL_PRE******************************//
			case I_GLOBAL_PRE:

			break;

	//************************I_GLOBAL_POST******************************//
			case I_GLOBAL_POST:
				sprintf(postBuffer,"#PRE%d",postCounter);
				postCounter++;
				while(1){			//search through instructions until the same label is found
					if(list->active->Instruction.instType == 28 && strcmp(list->active->Instruction.addr1, postBuffer)==0){
						break;
					}
					if(list->active->Instruction.instType == 30) {
						strcpy(currentClass, list->active->Instruction.addr1);
					}
					if(list->active->nextItem == NULL){		//no #PRE0 wrapper was found, starting in Main.run
						strcpy(currentClass, "Main");
						strcpy(currentFunc, "run");

						listFirst(list);
						while(1){			//search through instructions until the same label is found
							if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, "Main.run")==0){
								break;
							}
							listNext(list);
						}

						break;
					}
					listNext(list);
				}
				break;

	//************************I_CLASS:******************************//
			case I_CLASS:
				strcpy(currentClass, list->active->Instruction.addr1);
			break;

	//************************I_READ_INT******************************//
			case I_READ_INT:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_READ_INT. Expression based on nonexistant variable\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 28){		//Types are not matching, error4
						//printHtabLocal(localVarTable);
						fprintf(stderr,"I_READ_INT  target type not int.\n");
						memfreeall();
						exit(4);
					}
					if(itemPtr->varType != 28){memfreeall(); fprintf(stderr,"I_READ_INT. Target variable has incorrect type.\n");exit(4);}
					itemPtr->intValue = readInt();
					itemPtr->isInit = 1;
				}else{
					readInt();
				}

			break;

	//************************I_READ_STRING******************************//
			case I_READ_STRING:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_READ_STRING . Expression based on nonexistant variable\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 30){		//Types are not matching, error4
						fprintf(stderr,"I_READ_STRING  target type not string.\n");
						memfreeall();
						exit(4);
					}
					char * tmpRead = readString();
					itemPtr->stringValue=memalloc(sizeof(char)*(strlen(tmpRead) +1));
					strcpy(itemPtr->stringValue,tmpRead);
					itemPtr->isInit = 1;
				}else{
					readString();
				}

			break;

	//************************I_READ_DOUBLE******************************//
			case I_READ_DOUBLE:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_READ_DOUBLE. Expression based on nonexistant variable\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 23){		//Types are not matching, error4
						fprintf(stderr,"I_READ_DOUBLE  target type not double.\n");
						memfreeall();
						exit(4);
					}
					itemPtr->doubleValue = readDouble();
					itemPtr->isInit = 1;
				}else{
					readDouble();
				}
			break;

	//************************I_PRINT******************************//
			case I_PRINT:;
				int opt = atoi(list->active->Instruction.addr2);
				print(list->active->Instruction.addr1,opt,localVarTable,currentClass);
			break;

	//************************I_LENGTH******************************//
			case I_LENGTH:;
				int lengthInt = atoi(list->active->Instruction.addr2);
				if(strcmp(list->active->Instruction.addr3,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
						}
						if((itemPtr3=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_LENGTH. Argument variable not found.\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr3->varType != 28){memfreeall(); fprintf(stderr,"I_LENGTH. Target variable has incorrect type.\n");exit(4);}
					itemPtr3->intValue = length(list->active->Instruction.addr1,lengthInt,localVarTable,currentClass);
					itemPtr3->isInit = 1;
				}else{
					length(list->active->Instruction.addr1,lengthInt,localVarTable,currentClass);
				}
				

			break;

	//************************I_SUBSTR1******************************//
			case I_SUBSTR1:;
				int substrInt1 = atoi(list->active->Instruction.addr2);
				char* substrChar1 = NULL;
				substrChar1=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(substrChar1,list->active->Instruction.addr1);
			break;

	//************************I_SUBSTR2******************************//
			case I_SUBSTR2:;
				int substrInt2 = atoi(list->active->Instruction.addr2);
				char* substrChar2 = NULL;
				substrChar2=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(substrChar2,list->active->Instruction.addr1);

			break;

	//************************I_SUBSTR3******************************//
			case I_SUBSTR3:;
				int substrInt3 = atoi(list->active->Instruction.addr2);
				char* substrChar3 = NULL;
				substrChar3=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(substrChar3,list->active->Instruction.addr1);

			break;

	//************************I_SUBSTR4******************************//
			case I_SUBSTR4:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_SUBSTR. Variable not found.\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 30){memfreeall(); fprintf(stderr,"I_SUBSTR. Target variable has incorrect type.\n");exit(4);}
					char *substrPtr=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));	
					substrPtr=substr(substrChar1,substrInt1,substrChar2,substrInt2,substrChar3,substrInt3,localVarTable,currentClass);
					itemPtr->stringValue = memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
					strcpy(itemPtr->stringValue,substrPtr);	
					itemPtr->isInit = 1;
				}else{
					substr(substrChar1,substrInt1,substrChar2,substrInt2,substrChar3,substrInt3,localVarTable,currentClass);
				}

			break;

	//************************I_COMPARE1******************************//
			case I_COMPARE1:;
				int compInt1 = atoi(list->active->Instruction.addr2);
				char* compChar1 = NULL;
				compChar1=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(compChar1,list->active->Instruction.addr1);

			break;

	//************************I_COMPARE2******************************//
			case I_COMPARE2:;
				int compInt2 = atoi(list->active->Instruction.addr2);
				char* compChar2 = NULL;
				compChar2=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(compChar2,list->active->Instruction.addr1);

			break;

	//************************I_COMPARE3******************************//
			case I_COMPARE3:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_LENGTH. Argument variable not found.\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 28){memfreeall(); fprintf(stderr,"I_COMPARE. Target variable has incorrect type.\n");exit(4);}
					itemPtr->intValue = compare(compChar1,compInt1,compChar2,compInt2,localVarTable,currentClass);
					itemPtr->isInit = 1;
				}else{
					compare(compChar1,compInt1,compChar2,compInt2,localVarTable,currentClass);
				}

			break;

	//************************I_FIND1*********************************//
			case I_FIND1:;
				int findInt1 = atoi(list->active->Instruction.addr2);
				char* findChar1 = NULL;
				findChar1=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(findChar1,list->active->Instruction.addr1);
			break;

	//************************I_FIND2*********************************//
			case I_FIND2:;
				int findInt2 = atoi(list->active->Instruction.addr2);
				char* findChar2 = NULL;
				findChar2=memalloc(sizeof(char)*(strlen(list->active->Instruction.addr1) +1));
				strcpy(findChar2,list->active->Instruction.addr1);

			break;

	//************************I_FIND3*********************************//
			case I_FIND3:
				if(strcmp(list->active->Instruction.addr1,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_LENGTH. Argument variable not found.\n");
							memfreeall();
							exit(3);
						}
					}
					if(itemPtr->varType != 28){memfreeall(); fprintf(stderr,"I_FIND. Target variable has incorrect type.\n");exit(4);}
					itemPtr->intValue = find(findChar1,findInt1,findChar2,findInt2,localVarTable,currentClass);
					itemPtr->isInit = 1;
				}else{
					find(findChar1,findInt1,findChar2,findInt2,localVarTable,currentClass);
				}

			break;

	//************************I_SORT******************************//
			case I_SORT:;
				int sortInt = atoi(list->active->Instruction.addr2);
				if(strcmp(list->active->Instruction.addr3,"")){			//the variable is searched for only if there is assign before FN call
					if((itemPtr3=(htabSearch(localVarTable,list->active->Instruction.addr3))) == NULL) {	//localVarTable search for var
						if(!strstr(list->active->Instruction.addr3,dot)){		//if called function is short identifier
							strcpy(list->active->Instruction.addr3,concat(list->active->Instruction.addr3,currentClass));	//the concat it with class name
						}
						if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr3))) == NULL){	//if not in local, search global
							fprintf(stderr,"I_SORT. Argument variable not found.\n");
							memfreeall();
							exit(3);
						}
					}
				}else{
					itemPtr3=NULL;
				}
				char * sortPtr = sort(list->active->Instruction.addr1,sortInt,localVarTable,currentClass);
				if(itemPtr3!=NULL){
					if(itemPtr3->varType != 30){memfreeall(); fprintf(stderr,"I_SORT. Target variable has incorrect type.\n");exit(4);}
					itemPtr3->stringValue=memalloc(sizeof(char)*(strlen(sortPtr) +1));
					strcpy(itemPtr3->stringValue,sortPtr);
					itemPtr3->isInit = 1;			
				}
				
			break;
			
	//************************I_DO_GOTO******************************//
			case I_DO_GOTO:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){		//if called function is short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_DO_GOTO expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}
				if(itemPtr->varType == 28 && itemPtr->intValue == 0)jumpBool=1;		//logical switch is INT
				else if(itemPtr->varType == 23 && itemPtr->doubleValue == 0)jumpBool=1;		//logical switch is double
				else if(itemPtr->varType == 18 && itemPtr->boolValue == 0)jumpBool=1;	//logical switch is bool
				else{jumpBool=0;}

				if(!jumpBool){
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
		//************************I_DELETE_VAR******************************//
			case I_DELETE_VAR:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) == NULL) {	//localVarTable search for var
					if(!strstr(list->active->Instruction.addr1,dot)){		//if short identifier
						strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));	//the concat it with class name
					}
					if((itemPtr=(htabSearch(resources->globalVarTable,list->active->Instruction.addr1))) == NULL){	//if not in local, search global
						fprintf(stderr,"Sem_Error. I_DELETE_VAR expression based on nonexistant variable\n");
						memfreeall();
						exit(3);
					}
				}
				
				htabDelete(localVarTable,list->active->Instruction.addr1);
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
		case I_GLOBAL_PRE:		fprintf(stderr,"  I_GLOBAL_PRE:\t\t"); 	break;
		case I_GLOBAL_POST:		fprintf(stderr,"  I_GLOBAL_POST:\t"); 	break;
		case I_CLASS:		fprintf(stderr,"  I_CLASS:\t"); 	break;
		case I_READ_INT:		fprintf(stderr,"  I_READ_INT:\t"); 	break;
		case I_READ_STRING:		fprintf(stderr,"  I_READ_STRING:\t"); 	break;
		case I_READ_DOUBLE:		fprintf(stderr,"  I_READ_DOUBLE:\t"); 	break;
		case I_PRINT:		fprintf(stderr,"  I_PRINT:\t"); 	break;
		case I_LENGTH:		fprintf(stderr,"  I_LENGTH:\t"); 	break;
		case I_SUBSTR1:		fprintf(stderr,"  I_SUBSTR1:\t"); 	break;
		case I_SUBSTR2:		fprintf(stderr,"  I_SUBSTR2:\t"); 	break;
		case I_SUBSTR3:		fprintf(stderr,"  I_SUBSTR3:\t"); 	break;
		case I_SUBSTR4:		fprintf(stderr,"  I_SUBSTR4:\t"); 	break;
		case I_COMPARE1:		fprintf(stderr,"  I_COMPARE1:\t"); 	break;
		case I_COMPARE2:		fprintf(stderr,"  I_COMPARE2:\t"); 	break;
		case I_COMPARE3:		fprintf(stderr,"  I_COMPARE3:\t"); 	break;
		case I_FIND1:		fprintf(stderr,"  I_FIND1:\t"); 	break;
		case I_FIND2:		fprintf(stderr,"  I_FIND2:\t"); 	break;
		case I_FIND3:		fprintf(stderr,"  I_FIND3:\t"); 	break;
		case I_SORT:		fprintf(stderr,"  I_SORT:\t"); 	break;
		case I_DO_GOTO:		fprintf(stderr,"  I_DO_GOTO:\t"); 	break;
		case I_DELETE_VAR:		fprintf(stderr,"  I_DELETE_VAR:\t"); 	break;
		
		default: fprintf(stderr," unknown instruciton found:\t"); 	break;
	}
}
