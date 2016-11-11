#include "interpret.h"

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
	
	
	interpretEval(list,localVarTable,globalVarTable);
	

	
return 0;	
}


void interpretEval(tListOfInstr *list, thTable* localVarTable,thTable* globalVarTable){
	struct listItem *lastActive;
	
	while(1){
		fprintf(stderr,"interpreting instr: %d\n",list->active->Instruction.instType);
		switch (list->active->Instruction.instType){


	//************************I_STOP******************************//
			case I_STOP:
				exit(0);

			break;
			
	//************************I_LABEL******************************//
			case I_LABEL:


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
				lastActive=list->active;	//save pointer to active instr
		fprintf(stderr,"foo\n");
				listFirst(list);			//reset active to first for label search
				while(1){
					if(list->active->Instruction.instType == 1 && strcmp(list->active->Instruction.addr1, lastActive->Instruction.addr1)==0){
						break;
					}	
					if(list->active->nextItem == NULL) break;
					listNext(list);
				}
				
				interpretEval(list,localVarTable,globalVarTable);
				list->active=lastActive;	//restore active position before fn call
		fprintf(stderr,"bar\n");
			break;		
	
	//************************I_PROGRAM******************************//
			case I_PROGRAM:
				listNext(list);
			break;
			
			
			default:
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
		default: fprintf(stderr," unknown instruciton found:\t"); 	break;
	}
}