#define MAX_STACK 1023

typedef struct{
	char arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

void stackPush(tStack* s,char inputChar);
void stackPop(tStack* s);
void stackTop(tStack* s, char* c);
int stackEmpty(tStack* s);
int runPrecedenceAnalysis();
void reduction(char c[]);