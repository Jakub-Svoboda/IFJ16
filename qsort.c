#include "qsort.h"



void quick_sort(char* str, int l, int r){
	if(l < r) {					//Stops recursion
		int left = l;					//Index of the first character
		int right = r;					//Index of the last character
		char pivot = *(str + left);				//Stores the value of the character to sort
		while(left < right){					
			while(left < right && pivot <= *(str+right)) {				
				right--;
			}
			if(left < right) {
				*(str+left) = *(str+right);
				*(str+right) = pivot;
			}
			while(left < right && pivot >= *(str+left)) {
				left++;
			}
			if(left < right) {
				*(str+right) = *(str+left);
				*(str+left) = pivot; 
			}
		}
		quick_sort(str, l, left-1);			//Recursive call of function for the left substring
		quick_sort(str, right+1, r);			//Recursive call of function for the right substring 
	}

	return;
}

void sort(char *s) {			//Wrapper
	int l = 0;
	int r = strlen(s) - 1;
	quick_sort(s, l, r);

	return;
}

int main(int argc, char **argv){				//For testing
	char* str = (char*)malloc(sizeof(char)*100);
	printf("Input a string: ");
	fgets(str, 99, stdin);
	printf("String: %s\n", str);
	sort(str);
	printf("%s\n",str);
	free(str);

	return 0;
}
