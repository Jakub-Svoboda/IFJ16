#include "interpret.h"
#include "scanner.h"

int runInterpret(tListOfInstr *list){
	fprintf(stderr,"\n");
	listFirst(list);
	
	while(list->active->Instruction.instType != 0){
		printInstType(list->active->Instruction.instType);
		fprintf(stderr,": %s, %s, %s\n", (list->active->Instruction.addr1),(list->active->Instruction.addr2),(list->active->Instruction.addr3));
		listNext(list);
	}
	printInstType(list->active->Instruction.instType);	//print the very last instr
	fprintf(stderr,"\n");
	
	listFirst(list);
	interpretEval(list);
	
	thTable * functionTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
	htabInit(localVarTable);
	
return 0;	
}




void interpretEval(tListOfInstr *list)
{
	switch (list->active->Instruction.instType){


//************************I_STOP******************************//
		case I_STOP:


		break;
//************************I_LABEL******************************//
		case I_LABEL:


		break;
//************************I_GOTO******************************//
		case I_GOTO:


		break;
//************************I_NEW_VAR******************************//
		case I_NEW_VAR:


		break;
//************************I_FN_END******************************//
		case I_FN_END:


		break;
//************************I_RETURN******************************//
		case I_RETURN:


		break;
//************************I_IF_GOTO******************************//
		case I_IF_GOTO:

		break;

		default:
		return;
	}

}


void printInstType(int instructionType){
	switch (instructionType){
		case I_STOP:		fprintf(stderr,"  I_STOP:\t\n"); 	break;
		case I_LABEL:		fprintf(stderr,"I_LABEL:\t"); 		break;
		case I_GOTO:		fprintf(stderr,"  I_GOTO:\t"); 		break;
		case I_NEW_VAR:		fprintf(stderr,"  I_NEW_VAR:\t"); 	break;
		case I_FN_END:		fprintf(stderr,"  I_FN_END:\t"); 	break;
		case I_RETURN:		fprintf(stderr,"  I_RETURN:\t"); 	break;
		case I_IF_GOTO:		fprintf(stderr,"  I_IF_GOTO:\t"); 	break;
	}
}