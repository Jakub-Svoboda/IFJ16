#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpret.h"

int firstRun(thTable * functionTable, thTable * globalVarTable, FILE * f);
void functionInsert(Token * tokenName, Token * tokenType,Token * tokenClass, thTable * functionTable);
void globalVarInsert(Token * tokenName, Token * tokenType,Token * tokenClass, thTable * globalVarTable);
