#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"
#include "firstRun.h"

int main(int argc, char *argv[]){
	if(argc==2)
	{
		thTable * functionTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(functionTable);
		thTable * globalVarTable = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
		htabInit(globalVarTable);
		
		FILE *file;
		file = fopen(argv[1], "r");	
		firstRun(functionTable,globalVarTable,file);
		fclose(file);
		
		FILE *f;
		f = fopen(argv[1], "r");
		int result = runSyntaxAnalysis(f);
		result = result; 	//TODO delete me
		fclose(f);
	}
	return 0;
}
