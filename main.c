#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
#include "firstRun.h"
#include "interpret.h"

resourceStruct* resources;

int main(int argc, char *argv[]){
	if(argc==2)
	{	
		resources=malloc(sizeof (resourceStruct));			//Do NOT memalloc
		resources->memList=malloc(sizeof(tList));			//DO NOT MEMALLOC
		resources->memList->First=NULL;
		tListOfInstr list;
		listInit(&list);	//pointer to the list of instructions
		resources->functionTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(resources->functionTable);
		resources->globalVarTable = memalloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(resources->globalVarTable);
		FILE *file;
		if ((file = fopen(argv[1], "r")) != NULL){	//Checks for nonexistant file 
			firstRun(file);
			fclose(file);
		}
		else{
			memfreeall();
			exit(99);								//exits if file does not exist
		}
		int result;
		FILE *f;
		if ((f = fopen(argv[1], "r"))!= NULL){		//Checks for nonexistant file 
			result = runSyntaxAnalysis(f,&list);
			result = result; 	//TODO delete me
			fclose(f);	
		}else{
			memfreeall();
			exit(99);								//exits if file does not exist
		}
		
		result =runInterpret(&list);
		
	}
	return 0;
}
