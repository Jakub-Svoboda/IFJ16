#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"

int main(int argc, char *argv[]){
	if(argc==2)
	{	
		tListOfInstr list;
		listInit(&list);	//pointer to the list of instructions
		
		thTable * functionTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(functionTable);
		thTable * globalVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(globalVarTable);
		
		FILE *file;
		if ((file = fopen(argv[1], "r")) != NULL){	//Checks for nonexistant file 
			firstRun(functionTable,globalVarTable,file);
			fclose(file);			
		}else{
			exit(99);								//exits if file does not exist
		}
		
		FILE *f;
		if ((f = fopen(argv[1], "r"))!= NULL){		//Checks for nonexistant file 
			int result = runSyntaxAnalysis(f,&list);
			result = result; 	//TODO delete me
			fclose(f);	
		}else{
			exit(99);								//exits if file does not exist
		}
	}
	return 0;
}
