#include "syntax.h"
int functionDeclare(Token* tokenPtr,FILE* f){
	return 0;
}

int classBody(Token* tokenPtr,FILE* f){
	int result;
	tokenPtr = getToken(f);
	switch (tokenPtr -> type){
		case token_bracketLeftCurly:
			printf("OK: bracketLeftCurly\n");		//TODO delete me
			tokenPtr = getToken(f);
			switch (tokenPtr -> type){
				case token_static:
					printf("OK: static\n");		//TODO delete me
					tokenPtr = getToken(f);
					switch (tokenPtr -> type){
						case token_void:
							printf("OK: void\n");		//TODO delete me
							result = functionDeclare(tokenPtr,f);
							break;
						case token_int:
							printf("OK: int\n");		//TODO delete me
							result = functionDeclare(tokenPtr,f);
							break;
						case token_double:	
							printf("OK: double\n");		//TODO delete me
							result = functionDeclare(tokenPtr,f);
							break;
						case token_string:
							printf("OK: string\n");		//TODO delete me
							result = functionDeclare(tokenPtr,f);
							break;
						default:
							printf("Fail: not void, int, double or string\n");	//TODO delete me
					}
					break;	
				default:
					printf("Fail: not static\n");		//TODO delete me
					break;
			}
			break;
		default:
			printf("Fail: not left curly bracket\n");	//TODO delete me
	}
	return result;
}
int class(Token * tokenPtr,FILE* f){
	int result;
	tokenPtr = getToken(f);
	switch (tokenPtr -> type){
		case token_identifier:
			printf("OK: identifier \n");		//TODO delete me
			result = classBody(tokenPtr,f);
			break;
		case token_EOF:
			break;
		default:
			printf("fail: not class\n");	//TODO delete me
	}
	return result;
}

int classBlock(FILE * f){
	int result;
	Token * tokenPtr = getToken(f);
	switch (tokenPtr -> type){
		case token_class:
			printf("OK: class\n");		//TODO delete me
			result =class(tokenPtr,f);
			break;
		default:
			printf("Fail: not class\n");	//TODO delete me
	}
	free(tokenPtr);
	return result;
}



int runSyntaxAnalysis (FILE *f) {
	
	int result = classBlock(f);
	result =result; //TODO delete me
	
	return 0;
}






















