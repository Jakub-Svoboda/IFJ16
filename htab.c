#include "htab.h"

int hashFun(char* key) {
    int hash = 0;
    for(int i = 0; i < strlen(key); i++) {
        hash += key[i];
    }
    return hash % HTAB_SIZE;
}

void htabInit(thtabItem* htab[]) {
    for (int i = 0; i < HTAB_SIZE; i++) {
        htab[i] = NULL;
    }
}

void htabInsert(thtabItem* htab[], char* key) {
    thtabItem* item = (thtabItem*)malloc(sizeof(thtabItem));
    item->key = key;
    item->next = NULL;
    htab[hashFun(key)] = item;
}



int main() {
    thtabItem* htab[HTAB_SIZE];
    htabInit(htab);
    char* kocka = "kocka";
    printf("HASH KOCKA %d\n", hashFun(kocka));

    htabInsert(htab, kocka);

    for(int i = 0; i< HTAB_SIZE; i++) {
        if(htab[i] == NULL) printf("HTAB %d\n",i);
    }
    return 0;
}
