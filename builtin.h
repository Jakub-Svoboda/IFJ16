#include <stdio.h>			//TODO check if this is ok or BS

#define String char*  //Is this rhe right solution?


int readInt();
double readDouble();
String readString();
void print();
int length(String s);
String substr(String s, int i, int n);
int compare(String s1, String s2);
int find(String s, String search);
String sort(String s);