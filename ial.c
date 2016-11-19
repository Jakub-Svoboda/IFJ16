/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    ial.c
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include "ial.h"



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

int min(int a, int b) {			//Function returns smaller integer
	if(a < b) {
		return(a);
	}
	else {
		return(b);
	}
}

int max(int a, int b) {			//Function returns bigger integer 
	if(a > b) {
		return(a);
	}
	else {
		return(b);
	}
}

void compute_jumps(char *P, int *CharJump) {			//"Bad character rule"(first heuristics) Function computes maximal possible jump for each char
	int P_len = strlen(P);

	for(int i = 0; i < 256; i++) {			//Assigns maximum jump length for each char
		CharJump[i] = P_len;
	}

	for(int i = 0; i < P_len; i++) {			//For each char of pattern, compute maximum possible jump (how far can you skip to match current char in T with char in P)
		CharJump[(unsigned char)*(P + i)] = P_len - i - 1;
	}
}

void compute_match_jump(char *P, int *MatchJump) {			//"Good suffix rule"(second heuristics) Upon mismatch, function checks if matched suffix occurs in pattern P, if so, function shifts P until the other suffixes do match.
	int P_len = strlen(P);

	int m = P_len;
	int k = 0;
	int q = 0;
	int qq = 0;

	int Backup[P_len];

	for(k = 0; k < m; k++) {			//Fills array with maximum possible jump
		MatchJump[k] = 2 * m - k - 1;
	}

	k = m - 1;			//Index k points to the last character of pattern P
	q = m;			//Index q = pattern length

	while(k >= 0) {			//While k is not negative
		Backup[k] = q;			//Backup q on index k
		
		while((q < m) && (*(P + k) != *(P + q))) {			//While q < m and characters on these pointers are not equal
			
			MatchJump[q] = min(MatchJump[q], m - k - 1);			//Matchjump on index q = smaller value of MatchJump[q] and pattern length - k - 1
			q = Backup[q];			//Load value of q from backup
		}
		k--;			//Decrement index k
		q--;			//Decrement index q
	}

	for(k = 0; k <= q; k++) {			//For k = 0 to q

		MatchJump[k] = min(MatchJump[k], m + q - k);			//Matchjump on index k = smaller value of MatchJump[k] and pattern length + q - k
	}

	qq = Backup[q];			//qq = saved value on index q

	while(q < m - 1) {			//While q < pattern length - 1
		
		while(q <= qq) {			//While q is less than or equal to qq
			
			MatchJump[q] = min(MatchJump[q], qq - q + m);			//Matchjump on index q = smaller value of MatchJump[q] and qq - q + pattern length
			q++;			//Increment q
		}
		qq = Backup[qq];			//Load qq from backup
	}
}

int bma(char *P, char *T, int *CharJump, int *MatchJump) {			//Function returns index of match, or T length + 1
	int P_len = strlen(P);
	int T_len = strlen(T);

	int j = P_len - 1;
	int k = P_len - 1;

	compute_jumps(P, CharJump);			//Computes jumps for the first heuristic of bma
	compute_match_jump(P, MatchJump);			//Computes jumps for the second heuristic of bma

	while(j < T_len && k >= 0) {			//Checks each character (right to left) of P and compares it with characters in T

		if(T[j] == P[k]) {			//If characters do match, decrement indexes (to compare the next char)
			j--;
			k--;
		}
		else {			//If characters do not match, find the highest jump possible and jump
			
			j = j + max(CharJump[(unsigned char)*(T + j)], MatchJump[k]);
			k = P_len - 1;
		}	
	}

	if((k + 1) == 0) {			//If index in pattern P is -1 (Pattern was found), returns its index
		return(j + 1);
	}
	else {
		return(T_len + 1);			//Else returns T length + 1
	}
}

/*int main(int argc, char **argv) {			//For testing
	char* str = (char*)malloc(sizeof(char)*100);
    printf("Input a string: ");
    fgets(str, 99, stdin);
    printf("String: %s\n", str);
    int left = 0;
	int right = strlen(str) - 1;
	quick_sort(str, left, right);
    printf("%s\n",str);
    free(str);

	char *T = "qhojojahoj";
	char *P = "ahoj";
	int P_len = strlen(P);
	int CharJump[256];
	int MatchJump[P_len];

	int res = bma(P, T, CharJump, MatchJump);
	
	printf("%d\n", res);

	return 0;
}*/









//******************************HTAB***********************************//
//#define memalloc malloc
extern resourceStruct * resources;

char* concat(char* str1, char* str2) {          //Basic concatenate function to make CLASS.ID into one string
    int str1len = strlen(str1);
    int str2len = strlen(str2);
    char *buff = (char*) memalloc((str1len+str2len+2) * sizeof(char));        //Malloc memory of ideal length
    strcpy(buff,str2);
    strcat(buff,".");
    strcat(buff,str1);
    return buff;                               //return string
}

int hashFun(char* key) {                                    //this will hash your key into value of integer
    int hash = 0;
    int length = strlen(key);
    for(int i = 0; i < length; i++) {
        hash += key[i];                                     //sum of ord. value of each char
    }
    return hash % HTAB_SIZE;                                //return sum % (number of rows in hashtable)
}

void htabInit(thTable *htab) {
    for (int i = 0; i < HTAB_SIZE; i++) {
        (*htab)[i] = NULL;                                     //initialize array of NULL's with size of HTAB_SIZE
    }
}

void htabDeleteHashtag(thTable *htab) {
    char* search = htabSearchHashtag(htab);                     //temp string
    while( search[0] != '|') {                                  //while temp strin is not '|'
        htabDelete(htab, search);                               //delete item with this key
        search = htabSearchHashtag(htab);                       //new search so it can be deleted again
    }
}


char* htabSearchHashtag(thTable *htab) {

    if (*htab == NULL) {       //return NULL if hashtable or matching row is not initialized
        return NULL;
    }else {                          //simple debug...
        for (int i = 0; i < HTAB_SIZE; i++) {
            thtabItem* tempItem =(*htab)[i];
    		while(tempItem != NULL) {                                      //cycle through row
    			if(tempItem->key[0] == '#')  {return tempItem->key;}       //return item key that we were looking for
    			tempItem = tempItem->next;
            }
        }
        //    printf("foo2\n");
        return "|";                                                     //or end point
    }
}

thtabItem* htabSearch(thTable *htab, char* key) {

    if (*htab == NULL || (*htab)[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        //if ((*htab)[hashFun(key)] == NULL) printf("ssg\n");
        //if ((*htab)[hashFun(key)] == NULL) printf("kekfel\n");
        return NULL;
    }else {                          //simple debug...
        thtabItem* tempItem = (*htab)[hashFun(key)];           //assign first item of hashtable row into tempItem
		while(tempItem != NULL) {                             //cycle through row
			if(strcmp(tempItem->key,key)==0)  {return tempItem;}       //return item that we were looking for
			tempItem = tempItem->next;
        }
        //    printf("foo2\n");
        return tempItem;                                        //or NULL
    }
}

thtabItem* htabSearchClass(thTable *htab, char* key, char* classKey) {
    if (*htab == NULL || (*htab)[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        return NULL;
    }else {                            //simple debug...
        thtabItem* tempItem = (*htab)[hashFun(key)];           //assign first item of hashtable row into tempItem
        while(tempItem != NULL) {                           //cycle through row
            if(strcmp(tempItem->key,key)==0) {
                if(strcmp(tempItem->classKey,classKey)==0)return tempItem;
            }       //return item that we were looking for
            tempItem = tempItem->next;
        }
        return tempItem;                                        //or NULL
    }

}

void htabInsertReturnType(thTable *htab, char* funcName, char* classKey, Token_type ret) {
    //printf("v htabInsertReturnType\n");
    if (*htab == NULL) {

    }else {

        char *conKey = concat(funcName, classKey);
        thtabItem* tempItem = htabSearch(htab, conKey);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        if (tempItem == NULL) {

            if ((*htab)[hashFun(conKey)] == NULL) {               //hashtable row is not created yet
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = funcName;
                insertItem->next = NULL;
                insertItem->classKey = classKey;
                insertItem->returnType = ret;
                //insertItem->returnType = token;
                (*htab)[hashFun(conKey)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = funcName;
                insertItem->next = (*htab)[hashFun(conKey)];
                insertItem->classKey = classKey;
                insertItem->returnType = ret;
                //insertItem->returnType = token;
                (*htab)[hashFun(conKey)] = insertItem;                             //hashtable has already created row
            }
        }else {
            fprintf(stderr, "Error function redeclaration.\n");
            memfreeall();
            exit(3);
             //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            //tempItem->returnType = ret;
            //tempItem->classKey = classKey;
        }
    }
}

void htabInsertVarType(char* varName, char* classKey, Token_type var) {
    if (*(resources->globalVarTable) == NULL) {

    }else {
        char *conKey = concat(varName, classKey);
        thtabItem* tempItem = htabSearch(resources->globalVarTable, conKey);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        if (tempItem == NULL) {

            if ((*resources->globalVarTable)[hashFun(conKey)] == NULL) {               //hashtable row is not created yet
                //printf("novy v htabInsertReturnType\n");
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = varName;
                insertItem->next = NULL;
                insertItem->classKey = classKey;
                insertItem->varType = var;
                //insertItem->returnType = token;
                (*resources->globalVarTable)[hashFun(conKey)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = varName;
                insertItem->next = (*resources->globalVarTable)[hashFun(conKey)];
                insertItem->classKey = classKey;
                insertItem->varType = var;
                //insertItem->returnType = token;
                (*resources->globalVarTable)[hashFun(conKey)] = insertItem;                             //hashtable has already created row
            }
        }else {
            fprintf(stderr, "Error variable redeclaration.\n");
            memfreeall();
            exit(3);
            //printf("edit v htabInsertReturnType\n");
             //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            //tempItem->returnType = ret;
            //tempItem->classKey = classKey;
        }
    }
}

void htabInsert(thTable *htab, char* key, int localVarType) {
    if (*htab == NULL) {

        //printf("ss\n");
    }else {
		//printf("%s\n\n",key);
        thtabItem* tempItem = htabSearch(htab, key);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        if (tempItem == NULL) {                               //item is not yet in hashtable
            if ((*htab)[hashFun(key)] == NULL) {              //hashtable row is not created yet
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));
                insertItem->key = key;
                insertItem->next = NULL;
                insertItem->varType=localVarType;
                insertItem->argumentNumber=0;		//kuba edit
				//insertItem->returnType = token;
                (*htab)[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                                                        //hashtable has already created row
                thtabItem* insertItem = (thtabItem*)memalloc(sizeof(thtabItem));
                insertItem->key = key;
                //insertItem->returnType = token_invalid;
                insertItem->next = (*htab)[hashFun(key)];      //link pointer to next item of created item to 1st item of hashtable row
                insertItem->varType=localVarType;
                (*htab)[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }
        }else {
            ; //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            fprintf(stderr, "Semanticky error htab\n");
            memfreeall();
            exit(3);
        }

    }
}

//commented because of warnings
//const char* htabRead (thTable *htab, char* key) {
//    ; //used to read length of string etc if needed,
//}

void htabDelete (thTable *htab, char* key) {                //Find by key and delete item
    thtabItem* prevItem = (*htab)[hashFun(key)];            //find first item of row
    if (*htab == NULL || prevItem == NULL) {
        ;
    }else {
        thtabItem* tempItem = htabSearch(htab, key);        //ellegant solution via htabSearch, why is this banned in IAL??
        if (tempItem != NULL) {
            if (prevItem == tempItem) {                     //item is first in row
                (*htab)[hashFun(key)] = tempItem->next;     //move pointer to first item of row to the next item
            }else {
                while(prevItem->next != tempItem) {         //If prevItem is not before item to delete, continue and move onto next item
                    prevItem = prevItem->next;
                }
                prevItem->next = tempItem->next;            //Item is found so skip it
            }
				//free(tempItem);                                 //FREI
        }
    }
}

//This will take htab back to state after initialization
void htabDispose(thTable *htab) {
    for (int i = 0; i< HTAB_SIZE; i++) {        //cycle through hashTable and delete/free all of them
        thtabItem* tempItem = (*htab)[i];
        //thtabItem* delItem;
        while(tempItem != NULL) {
            //delItem = tempItem;	//Kuba-edit, not needed to free now since we have a garbage collector
            tempItem = tempItem->next;
            //free(delItem);                      //FREI
        }
        (*htab)[i] = NULL;                      //initialize array of pointers to NULL
    }
}

void printHtab(thTable *htab, int var) {        // Multifunctional print function. for testing purposes
    //printf("aa\n");
    for (int i = 0; i< HTAB_SIZE; i++) {
        printf("|ROW %d|",i);
        thtabItem* temp = (*htab)[i];
        while(temp != NULL) {
            if (var) printf("->[%s %d %s ]",temp->key,temp -> varType, temp -> classKey);
            if (!var) printf("->[%s %d %s %s]",temp->key,temp -> returnType, temp -> classKey, temp->name);
            temp = temp->next;
        }
        printf("\n");
    }
}


void printHtabLocal(thTable *htab){                 //Simple EZ PZ print, don't use
    for (int i = 0; i< HTAB_SIZE; i++) {
        printf("|ROW %d|",i);
        thtabItem* temp = (*htab)[i];
        while(temp != NULL) {
            printf("->[%s type:%d i: %d d: %g s: %s b: %d]",temp->key,temp->varType,temp->intValue,temp->doubleValue,temp->stringValue,temp->boolValue);
            temp = temp->next;
        }
        printf("\n");
    }
}
/*
 //for testing purposes
int main() {
    thTable *htab = NULL;
    if(*htab == NULL) printf("laamo\n");
    htab = malloc(sizeof(struct thtabItem) * HTAB_SIZE);
    if(*htab == NULL) printf("null\n");
    htabInit(htab);

    char* kocka = "kocka";

    htabInsert(htab, "3kok", 0);                                          //few tests of insert
    htabInsert(htab, "c#akko",0);
    htabInsert(htab, "ca#kko",0);
    htabInsert(htab, "cak#ko",0);
    htabInsert(htab, "#cakko",0);
    htabInsert(htab, "cakko",0);

    htabInsert(htab, "slyse#",0);
    htabInsert(htab, "#sysel",0);
    htabInsert(htab, "slys#e",0);
    htabInsert(htab, "kacko",0);
    printHtabLocal(htab);
    htabDeleteHashtag(htab);
    printHtabLocal(htab);

    return 0;
}
*/
