#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef	FIRSTRUN_H
#define FIRSTRUN_H 

int firstRun(thTable * globalVarTable, FILE * f);
void functionInsert(Token * tokenName, Token * tokenType,Token * tokenClass);
void globalVarInsert(Token * tokenName, Token * tokenType,Token * tokenClass, thTable * globalVarTable);
#endif