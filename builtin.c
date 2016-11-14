#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"

int readInt() {
    ;
}

double readDouble() {
    ;
}

String readString() {
    ;
}



void print() {
    ;
}

int length(String s){
    char *p = s;
    while(*p) {
        p++;
    }
    return p-s;
}

String substr(String s, int i, int n) {
    if(length(s) < i+n) {
        fprintf(stderr, "Substring error, invalid values\n");
        exit(10);
    }
    char *buff = (char*) malloc(n * sizeof(char));					//ZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZOZ
    for(int x = 0; x < n; x++) {
        buff[x] = s[i+x];
    }
    return buff;
}

int compare(String s1, String s2) {
    ;
}

int find(String s, String search) {
    ;
}

String sort(String s) {
    ;
}

/*testing purposes
int main() {
    String s = "Kekel je z pekel.";
    printf("Zadarilo se a substr je: {%s}\n", substr(s, 2, 100));
    return 1;
}
*/
