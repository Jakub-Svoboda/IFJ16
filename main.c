#include <stdio.h>
#include <stdlib.h>
#include "syntax.h"

int main(int argc, char *argv[]){
	FILE *f;
	f = fopen(argv[1], "r");
	runPrecedenceAnalysis(f);
	fclose(f);
	return 0;
}
