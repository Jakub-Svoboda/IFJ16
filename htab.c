#include "htab.h"

char* concat(char* str1, char* str2) {


    int str1len = strlen(str1);
    int str2len = strlen(str2);
    char *buff = (char*) malloc((str1len+str2len+2) * sizeof(char));
    strcpy(buff,str2);
    strcat(buff,".");
    strcat(buff,str1);

    //printf("@@@@@%s",buff);
    return buff;
}

int hashFun(char* key) {                                    
    int hash = 0;
    int length = strlen(key);
    for(int i = 0; i < length; i++) {
        hash += key[i];                                     //sum of ord. value of each char
    }
    return hash % HTAB_SIZE;                                //return sum % (number of rows in hashtable)
}

void htabInit(thTable *htab) {
    //printf("ss\n");
    //*htab = malloc(sizeof(struct thtabItem)*HTAB_SIZE);
    for (int i = 0; i < HTAB_SIZE; i++) {
        (*htab)[i] = NULL;                                     //initialize array of NULL's with size of HTAB_SIZE
    }
}

thtabItem* htabSearch(thTable *htab, char* key) {

    if (*htab == NULL || (*htab)[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        //if ((*htab)[hashFun(key)] == NULL) printf("ssg\n");
        //if ((*htab)[hashFun(key)] == NULL) printf("kekfel\n");
        return NULL;
    }else {
        //printf("kekel\n");
        //printf("searching\n");                              //simple debug...
        thtabItem* tempItem = (*htab)[hashFun(key)];           //assign first item of hashtable row into tempItem
        //    printf("foo\n");
        while(tempItem != NULL) {                           //cycle through row
            if(strcmp(tempItem->key,key)==0) return tempItem;       //return item that we were looking for
            tempItem = tempItem->next;
        }
        //    printf("foo2\n");
        return tempItem;                                        //or NULL
    }

}

thtabItem* htabSearchClass(thTable *htab, char* key, char* classKey) {
    //printf("v htabSearchClass\n");
    if (*htab == NULL || (*htab)[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        //if ((*htab)[hashFun(key)] == NULL) printf("ssg\n");
        //if ((*htab)[hashFun(key)] == NULL) printf("kekoefel\n");
        return NULL;
    }else {
        //printf("kekel\n");
        //printf("searching\n");                              //simple debug...
        thtabItem* tempItem = (*htab)[hashFun(key)];           //assign first item of hashtable row into tempItem
        //    printf("foo\n");
        while(tempItem != NULL) {                           //cycle through row
            if(strcmp(tempItem->key,key)==0) {
                if(strcmp(tempItem->classKey,classKey)==0)return tempItem;
            }       //return item that we were looking for
            tempItem = tempItem->next;
        }
        //    printf("foo2\n");
        return tempItem;                                        //or NULL
    }

}

void htabInsertReturnType(thTable *htab, char* funcName, char* classKey, Token_type ret) {
    //printf("v htabInsertReturnType\n");
    if (*htab == NULL) {

    }else {
        char *conKey = concat(funcName, classKey);
        //printf("dvakrat v htabInsertReturnType\n");
        thtabItem* tempItem = htabSearch(htab, conKey);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        //printf("trikrat v htabInsertReturnType\n");
        if (tempItem == NULL) {

            if ((*htab)[hashFun(conKey)] == NULL) {               //hashtable row is not created yet
                //printf("novy v htabInsertReturnType\n");
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = funcName;
                insertItem->next = NULL;
                insertItem->classKey = classKey;
                insertItem->returnType = ret;
                //insertItem->returnType = token;
                (*htab)[hashFun(conKey)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));

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
            exit(3);
            //printf("edit v htabInsertReturnType\n");
             //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            //tempItem->returnType = ret;
            //tempItem->classKey = classKey;
        }
    }
}

void htabInsertVarType(thTable *htab, char* varName, char* classKey, Token_type var) {
    //printf("v htabInsertReturnType\n");
    if (*htab == NULL) {

    }else {
        char *conKey = concat(varName, classKey);
        //printf("dvakrat v htabInsertReturnType\n");
        thtabItem* tempItem = htabSearch(htab, conKey);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        //printf("trikrat v htabInsertReturnType\n");
        if (tempItem == NULL) {

            if ((*htab)[hashFun(conKey)] == NULL) {               //hashtable row is not created yet
                //printf("novy v htabInsertReturnType\n");
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = varName;
                insertItem->next = NULL;
                insertItem->classKey = classKey;
                insertItem->varType = var;
                //insertItem->returnType = token;
                (*htab)[hashFun(conKey)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));

                insertItem->key = conKey;
                insertItem->name = varName;
                insertItem->next = (*htab)[hashFun(conKey)];
                insertItem->classKey = classKey;
                insertItem->varType = var;
                //insertItem->returnType = token;
                (*htab)[hashFun(conKey)] = insertItem;                             //hashtable has already created row
            }
        }else {
            fprintf(stderr, "Error variable redeclaration.\n");
            exit(3);
            //printf("edit v htabInsertReturnType\n");
             //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            //tempItem->returnType = ret;
            //tempItem->classKey = classKey;
        }
    }
}

void htabInsert(thTable *htab, char* key) {
    if (*htab == NULL) {

        //printf("ss\n");
    }else {
		printf("%s\n\n",key);
        thtabItem* tempItem = htabSearch(htab, key);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        if (tempItem == NULL) {                             //item is not yet in hashtable
            if ((*htab)[hashFun(key)] == NULL) {               //hashtable row is not created yet
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));
                insertItem->key = key;
                insertItem->next = NULL;
                //insertItem->returnType = token;
                (*htab)[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }else {
                                                        //hashtable has already created row
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));
                insertItem->key = key;
                //insertItem->returnType = token_invalid;
                insertItem->next = (*htab)[hashFun(key)];      //link pointer to next item of created item to 1st item of hashtable row
                (*htab)[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }
        }else {
            ; //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
            fprintf(stderr, "Semanticky error htab\n");
            exit(3);
        }

    }
}

//commented because of warnings
//const char* htabRead (thTable *htab, char* key) {
//    ; //used to read length of string etc if needed,
//}

void htabDelete (thTable *htab, char* key) {
    thtabItem* prevItem = (*htab)[hashFun(key)];
    if (*htab == NULL || prevItem == NULL) {
        ;
    }else {
        thtabItem* tempItem = htabSearch(htab, key);        //ellegant solution via htabSearch, why is this banned in IAL??
        if (tempItem != NULL) {
            if (prevItem == tempItem) {                     //item is first in row
                (*htab)[hashFun(key)] = tempItem->next;
            }else {
                while(prevItem->next != tempItem) {
                    prevItem = prevItem->next;
                }
                prevItem->next = tempItem->next;
            }
            free(tempItem);
        }
    }
}

void htabDispose(thTable *htab) {
    for (int i = 0; i< HTAB_SIZE; i++) {
        thtabItem* tempItem = (*htab)[i];
        thtabItem* delItem;
        while(tempItem != NULL) {
            delItem = tempItem;
            tempItem = tempItem->next;
            free(delItem);
        }
        (*htab)[i] = NULL;
    }
}

void printHtab(thTable *htab, int var) {
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


void printHtabLocal(thTable *htab){
    for (int i = 0; i< HTAB_SIZE; i++) {
        printf("|ROW %d|",i);
        thtabItem* temp = (*htab)[i];
        while(temp != NULL) {
            printf("->[%s ]",temp->key);
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
    printf("HASH KOCKA %d\n", hashFun(kocka));                          //test hashFun

    htabInsert(htab, "kaock");                                          //few tests of insert
    htabInsert(htab, "cakko");
        htabInsert(htab, "sysel");
    htabInsert(htab, "kacko");
    htabInsert(htab, "sysel");

    htabInsert(htab, "televiza");
    htabInsert(htab, "balon");
    htabInsert(htab, kocka);


    if(htabSearch(htab, kocka) != NULL) printf("Kocka je tam\n");       //test search

    printHtab(htab);
    printf("\n");
    htabDelete(htab, "televiza");
    printHtab(htab);
    printf("\n");
    htabDelete(htab, "kaock");
    printHtab(htab);

    htabInsert(htab, "kaock");
    printf("\n");
    printHtab(htab);

    printf("\n");
    htabDispose(htab);
    printHtab(htab);
    return 0;
}
*/
