#include "interpret.h"

char *strtok_r(char *, const char *, char **);	//required for strtok_r function

int runInterpret(tListOfInstr *list,thTable * globalVarTable){
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
	thTable * localVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
	htabInit(localVarTable);
	
	fprintf(stderr,"\n");	//TODO comment me
	interpretEval(list,localVarTable,globalVarTable);
	

	
return 0;	
}

void interpretEval(tListOfInstr *list, thTable* localVarTable,thTable* globalVarTable){
	struct listItem *lastActive;
	thTable * nextCallTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);		//Alocate memory for var table, which will be passed to new function
	htabInit(nextCallTable);
	char currentFunc[2047];
	char currentClass[2047];
	char* dot=".";
	thtabItem* itemPtr = NULL;		//pointer to 1st operand
	thtabItem* itemPtr2 = NULL;		//pointer to 2nd operand
	
	while(1){
		fprintf(stderr,"interpreting instr: %d, %s, %s, %s\n",list->active->Instruction.instType,list->active->Instruction.addr1, list->active->Instruction.addr2,list->active->Instruction.addr3);
		switch (list->active->Instruction.instType){


	//************************I_STOP******************************//
			case I_STOP:
				
				fprintf(stderr,"\nInterpret over.\n\n");	//TODO comment me
				//memfreeall();
				exit(0);	//TODO free stuff

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


			break;
			
	//************************I_NEW_VAR******************************//
			case I_NEW_VAR:
				htabInsert(localVarTable, list->active->Instruction.addr1, atoi(list->active->Instruction.addr2));
			break;
			
	//************************I_FN_END******************************//
			case I_FN_END:


			break;
			
	//************************I_RETURN******************************//
			case I_RETURN:

				return;
			break;
			
	//************************I_IF_GOTO******************************//
			case I_IF_GOTO:

			break;
			
	//************************I_FN_CALL******************************//
			case I_FN_CALL:
				if(!strstr(list->active->Instruction.addr1,dot)){
					strcpy(list->active->Instruction.addr1,concat(list->active->Instruction.addr1,currentClass));
		//			fprintf(stderr,"\n%s\n\n\n", list->active->Instruction.addr1);
				}

				lastActive=list->active;	//save pointer to active instr
				listFirst(list);			//reset active to first for label search
				while(1){
					if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr1)==0){
						break;
					}	
					if(list->active->nextItem == NULL){
						fprintf(stderr,"\ncalled function not found\n\n\n"); 
						exit(3);		//TODO free		
					}
					listNext(list);
				}
				interpretEval(list,nextCallTable,globalVarTable);
				htabDispose(nextCallTable);
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
					itemPtr->intValue = atoi(list->active->Instruction.addr2);
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_INT to nonexistant variable.\n");
					exit(3);
				}
			break;		
			
	//************************I_MOV_DOUBLE******************************//
			case I_MOV_DOUBLE:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->doubleValue = atof(list->active->Instruction.addr2);
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_DOUBLE to nonexistant variable.\n");
					exit(3);
				}
			break;	
			
	//************************I_MOV_STRING******************************//
			case I_MOV_STRING:
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL){
					itemPtr->stringValue=malloc(0);		//TODO check what is going on, the strcpy chrashes without malloc, but malloc(0) and free works for some reason.
					strcpy(itemPtr->stringValue,list->active->Instruction.addr2);
				}else{
					fprintf(stderr,"Sem_Error. I_MOV_STRING to nonexistant variable.\n");
					exit(3);
				}
				printHtabLocal(localVarTable);
			break;	

	//************************I_ADD******************************//
			case I_ADD:

			break;
	
	//************************I_SUB******************************//
			case I_SUB:

			break;	
			
	//************************I_MUL******************************//
			case I_MUL:

			break;	
			
	//************************I_DIV******************************//
			case I_DIV:

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
				if((itemPtr=(htabSearch(localVarTable,list->active->Instruction.addr1))) != NULL && (itemPtr2=(htabSearch(localVarTable,list->active->Instruction.addr2))) != NULL){
					itemPtr->varType=itemPtr2->varType;		//copy operand2s type
					itemPtr->isInit=1;					//Mark the variable as initialized. It can be now used in expressions.
					switch (itemPtr2->varType){
						case 28:			//source is type int
							itemPtr->intValue=itemPtr2->intValue;
							break;
						case 30:			//source is type String
	
							break;
						case 23:			//source is type double
							itemPtr->doubleValue=itemPtr2->doubleValue;						
							break;
							
						default:

							fprintf(stderr,"I_MOV source operand has unexpected type %d\n",itemPtr->varType);
							exit(3);	//TODO this is our fault, exit code is hard to define....
					}
				}else{									//Variable is not in var table exist
					printHtabLocal(localVarTable);
					fprintf(stderr,"Sem_Error. I_MOV to nonexistant variable.\n");
					exit(3);
				}
			break;	

			break;	

	//************************I_WHILE_GOTO******************************//
			case I_WHILE_GOTO:

			break;		

	//************************I_RETURN_NOTHING******************************//
			case I_RETURN_NOTHING:

			break;				

	//************************I_PUSH******************************//
			case I_PUSH:

			break;	

			
			
			
			default:
				fprintf(stderr,"Interpreting an unknown instruction %d\n",list->active->Instruction.instType);
			return;
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
		default: fprintf(stderr," unknown instruciton found:\t"); 	break;
	}
}