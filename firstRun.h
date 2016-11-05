#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htab.h"

int firstRun(thTable * functionTable, thTable * globalVarTable, FILE * f);
void functionInsert(Token * tokenName, Token * tokenType, thTable * functionTable);
void globalVarInsert(Token * tokenName, Token * tokenType, thTable * functionTable);