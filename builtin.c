#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include "ial.h"
#include "garbage.h"

//////////////////////////////
//  AS SIMPLE AS IT CAN BE  //
//  SO PLEASE CHECK IT      //
//////////////////////////////

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
    if(length(s) < i+n || i < 0 || n < 0) {
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
    char *T = s;
    char *P = search;

    int P_len = strlen(P);
    int T_len = strlen(T);
    int CharJump[256];
    int MatchJump[P_len];

    int res = bma(P, T, CharJump, MatchJump);

    if(res < T_len) {
        return(res);
    }
    else {
        return(-1);
    }
}

String sort(String s) {
	int s_len = strlen(s);
    char *str = (char*)malloc(sizeof(char)*s_len);          //Does not work whem memalloc is used, don't know why..
    for(int x = 0; x < s_len; x++) {
        str[x] = s[x];
    }
    int left = 0;
    int right = strlen(str) - 1;
    quick_sort(str, left, right);

    return(str);
}

//testing purposes
/*
int main() {
	String k = "qhojojahoz";
	String l = "z";

    int res = find(k, l);
    printf("%d\n", res);

    String sorted = sort(k);
    printf("Serazeny retezec: {%s}\n", sorted);


    String s = "Kekel";
    printf("Zadarilo se a substr je: {%s}\n", substr(s, 0, 4));
    return 1;
}
*/
