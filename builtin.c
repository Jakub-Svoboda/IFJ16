#include <stdio.h>
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
    ;
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
