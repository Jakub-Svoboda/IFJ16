#define MAX_STACK 1023

typedef struct{
	char arr[MAX_STACK][1];		//1024 2 when indexed from 0
	int top;
}tStack;

int runSyntax();