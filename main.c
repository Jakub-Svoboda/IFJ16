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
		}
		else{
			exit(99);								//exits if file does not exist
		}
		char *key="Main.run";						//initialize variable for Main.run existence check
		if(htabSearch(functionTable, key)==NULL){	//if function run in class Main does not exist
				fprintf(stderr, "Cannot find funtion \"run\" in class \"Main\"\n");		//print error and exit
				exit(3);					//TODO call garbage collector
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
