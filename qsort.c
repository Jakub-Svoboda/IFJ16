#include "qsort.h"



void partition(char *A, int left, int right, int *i, int *j) {
char PM;
	char tmp;
	
	*i = left;
	*j = right;
	PM = *(A + ((*i + *j) / 2));

	while(*i <= *j) {

		while(*(A + *i) < PM) {
			*i = *i + 1;
		}

		while(*(A + *j) > PM) {
			*j = *j - 1;
		}

		if(*i <= *j) {
			tmp = *(A + *i);
			*(A + *i) = *(A + *j);
			*(A + *j) = tmp;
			*i = *i + 1;
			*j = *j - 1;
		}
	}
}

void quick_sort(char *A, int left, int right) {
	int i = 0;
	int j = 0;

	partition(A, left, right, &i, &j);

	if(left < j) {
		quick_sort(A, left, j);
	}

	if(i < right) {
		quick_sort(A, i, right);
	}
}

int main(int argc, char **argv){                                //For testing
        char* str = (char*)malloc(sizeof(char)*100);
        printf("Input a string: ");
        fgets(str, 99, stdin);
        printf("String: %s\n", str);
        int left = 0;
	int right = strlen(str) - 1;
	quick_sort(str, left, right);
        printf("%s\n",str);
        free(str);

        return 0;
}

