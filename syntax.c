#include "syntax.h"

int runSyntaxAnalysis (FILE *f) {
	
	int result;
	Token * tokenPtr = getToken(f);
	printf("foo\n");
	switch (tokenPtr -> type)	{
		case token_class:
			printf("Jsme sikulky\n");
			break;
		default:
			printf("You are a moron\n");	//TODO delete me
	}
		
	return 0;
}






















