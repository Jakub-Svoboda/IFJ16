#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"

resourceStruct* resources;

int main(int argc, char *argv[]){
	if(argc==2)
	{	
		resources=malloc(sizeof (resourceStruct));
		tListOfInstr list;
		listInit(&list);	//pointer to the list of instructions
		fprintf(stderr,"foo\n");
		resources->functionTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		fprintf(stderr,"foo\n");
		htabInit(resources->functionTable);
		thTable * globalVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(globalVarTable);
		FILE *file;
		if ((file = fopen(argv[1], "r")) != NULL){	//Checks for nonexistant file 
			firstRun(globalVarTable,file);
			fclose(file);
		char *key="Main.run";						//initialize variable for Main.run existence check
		if(htabSearch(resources->functionTable, key)==NULL){	//if function run in class Main does not exist
				fprintf(stderr, "Cannot find funtion \"run\" in class \"Main\"\n");		//print error and exit
				exit(3);					//TODO call garbage collector
		}			
		}
		else{
			exit(99);								//exits if file does not exist
		}

		int result;
		FILE *f;
		if ((f = fopen(argv[1], "r"))!= NULL){		//Checks for nonexistant file 
			result = runSyntaxAnalysis(f,&list);
			result = result; 	//TODO delete me
			fclose(f);	
		}else{
			exit(99);								//exits if file does not exist
		}
		
		result =runInterpret(&list,globalVarTable);
		
	}
	return 0;
}
