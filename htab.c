#include "htab.h"
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
		while(tempItem != NULL) {                           //cycle through row
			if(strcmp(tempItem->key,key)==0)  {return tempItem;}       //return item that we were looking for
			tempItem = tempItem->next;
        }
        //    printf("foo2\n");
        return tempItem;                                        //or NULL
    }
}

thtabItem* htabSearchClass(thTable *htab, char* key, char* classKey) {
    if (*htab == NULL || (*htab)[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        //if ((*htab)[hashFun(key)] == NULL) printf("ssg\n");
        //if ((*htab)[hashFun(key)] == NULL) printf("kekoefel\n");
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
