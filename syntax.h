#define MAX_STACK 1023

typedef struct{
	struct Token *arr[MAX_STACK];		//1024 2 when indexed from 0
	int top;
}tStack;

void stackPush(tStack *s,struct Token *Token);
void stackPop(tStack* s);
void stackTop(tStack *s, struct Token *Token);
int stackEmpty(tStack* s);
int runPrecedenceAnalysis();
void reduction(char c[]);