#include "qsort.h"



void partition(char *A, int left, int right, int *i, int *j) {			//Function divides array to two subarrays. Left subarray has smaller values than median, right has higher.
	char PM;			//Declaration of pseudomedian.
	char tmp;			//Temporary variable for swaping values.
	
	*i = left;			//Initialization of i.
	*j = right;			//Initialization of j.
	PM = *(A + ((*i + *j) / 2));			//Setting pseudomedian.

	while(*i <= *j) {

		while(*(A + *i) < PM) {			//Increments index i while value on index i is smaller than pseudomedian.
			*i = *i + 1;
		}

		while(*(A + *j) > PM) {			//Increments index j while value on index j is bigger than pseudomedian.
			*j = *j - 1;
		}

		if(*i <= *j) {			//If i is lower than or equeal to j, swaps values on these indexes.
			tmp = *(A + *i);
			*(A + *i) = *(A + *j);
			*(A + *j) = tmp;
			*i = *i + 1;
			*j = *j - 1;
		}
	}
}

void quick_sort(char *A, int left, int right) {			//Function sorts array given.
	int i = 0;			//Index
	int j = 0;			//Index

	partition(A, left, right, &i, &j);			//Partitioning array.

	if(left < j) {
		quick_sort(A, left, j);			//Recursive call of function for left subarray.
	}

	if(i < right) {
		quick_sort(A, i, right);			//Recursive call of function for right subarray.
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

