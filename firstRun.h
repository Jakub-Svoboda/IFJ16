#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htab.h"
#include "scanner.h"

int firstRun(thTable * functionTable, thTable * globalVarTable, FILE * f);
void functionInsert(Token * tokenPtr, thTable * functionTable);
void globalVarInsert(Token * tokenPtr, thTable * globalVarTable);