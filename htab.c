#include "htab.h"

int hashFun(const char* key) {                                    //Drugs are fun
    int hash = 0;
    for(int i = 0; i < strlen(key); i++) {
        hash += key[i];                                     //sum of ord. value of each char
    }
    return hash % HTAB_SIZE;                                //return sum % (number of rows in hashtable)
}

void htabInit(thTable htab) {
    for (int i = 0; i < HTAB_SIZE; i++) {
        htab[i] = NULL;                                     //initialize array of NULL's with size of HTAB_SIZE
    }
}

thtabItem* htabSearch(thTable htab, const char* key) {

    if (htab == NULL || htab[hashFun(key)] == NULL) {       //return NULL if hashtable or matching row is not initialized
        return NULL;
    }else {
//      printf("searching\n");                              //simple debug...
        thtabItem* tempItem = htab[hashFun(key)];           //assign first item of hashtable row into tempItem
        while(tempItem != NULL) {                           //cycle through row
            if(tempItem->key == key) return tempItem;       //return item that we were looking for
            tempItem = tempItem->next;
        }
        return NULL;                                        //or NULL
    }

}

void htabInsert(thTable htab, const char* key) {
    if (htab == NULL) {
        ;
    }else {
        thtabItem* tempItem = htabSearch(htab, key);          //EDIT reminder (thtabItem*)malloc(sizeof(thtabItem));
        if (tempItem == NULL) {                             //item is not yet in hashtable
            if (htab[hashFun(key)] == NULL) {               //hashtable row is not created yet
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));
                insertItem->key = key;
                insertItem->next = NULL;
                htab[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }else {                                         //hashtable has already created row
                thtabItem* insertItem = (thtabItem*)malloc(sizeof(thtabItem));
                insertItem->key = key;
                insertItem->next = htab[hashFun(key)];      //link pointer to next item of created item to 1st item of hashtable row
                htab[hashFun(key)] = insertItem;            //insert created item on the first position in hashtable row
            }
        }else {
            ; //item is already in hashtable, but it's not possible for scanner to actualize value if the only value is key,. now what?
        }

    }
}

const char* htabRead (thTable htab, const char* key) {
    ; //used to read length of string etc if needed,
}

void htabDelete (thTable htab, const char* key) {
    thtabItem* prevItem = htab[hashFun(key)];
    if (htab == NULL || prevItem == NULL) {
        ;
    }else {
        thtabItem* tempItem = htabSearch(htab, key);        //ellegant solution via htabSearch, why is this banned in IAL??
        if (tempItem != NULL) {
            if (prevItem == tempItem) {                     //item is first in row
                htab[hashFun(key)] = tempItem->next;
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

void htabDispose(thTable htab) {
    for (int i = 0; i< HTAB_SIZE; i++) {
        thtabItem* tempItem = htab[i];
        thtabItem* delItem;
        while(tempItem != NULL) {
            delItem = tempItem;
            tempItem = tempItem->next;
            free(delItem);
        }
        htab[i] = NULL;
    }
}

void printHtab(thTable htab) {
    for (int i = 0; i< HTAB_SIZE; i++) {
        printf("|ROW %d|",i);
        thtabItem* temp = htab[i];
        while(temp != NULL) {
            printf("->[%s]",temp->key);
            temp = temp->next;
        }
        printf("\n");
    }
}


int main() {
    thTable htab;
    htabInit(htab);

    const char* kocka = "kocka";
    printf("HASH KOCKA %d\n", hashFun(kocka));                          //test hashFun

    htabInsert(htab, "kaock");                                          //few tests of insert
    htabInsert(htab, "cakko");
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
