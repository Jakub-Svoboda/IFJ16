#include "interpret.h"
#include "scanner.h"

int runInterpret(tListOfInstr *list){
	fprintf(stderr,"\n");
	listFirst(list);
	
	while(list->active->Instruction.instType != 0){
		fprintf(stderr,"type %d",list->active->Instruction.instType );
		fprintf(stderr,": %s, %s, %s\n", (list->active->Instruction.addr1),(list->active->Instruction.addr2),(list->active->Instruction.addr3));
		listNext(list);
	}

	
	
return 0;	
}