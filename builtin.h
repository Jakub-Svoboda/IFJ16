#include <stdio.h>			//TODO check if this is ok or BS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ial.h"
#include "garbage.h"
#include "htab.h"

#define String char*  //Is this rhe right solution?
#define LOCAL_BUFF_SIZE 32

int readInt();
double readDouble();
String readString();
//char* printCreate(Token *t);
void print();
int length(String s, int stringOpt);
String substr(String s, int stringOpt, char* iNum, int iOpt, char* nNum, int nOpt, thTable *htab, char* class);
int compare(String s1, int s1Opt, String s2, int s2Opt, thTable *htab, char* class);
int find(String s, int stringOpt, String search, int searchOpt, thTable *htab, char* class);
String sort(String s, int stringOpt, thTable *htab, char* class)
