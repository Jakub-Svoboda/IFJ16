#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"

int main(int argc, char *argv[]){
	if(argc==2)
	{
		FILE *f;
		f = fopen(argv[1], "r");
		int result = runSyntaxAnalysis(f);
		fclose(f);
	}
	return 0;
}
