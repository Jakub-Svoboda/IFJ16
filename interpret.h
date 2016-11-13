#include <stdio.h>
#include <stdlib.h>
#include "iList.h"
#include "htab.h"


int runInterpret(tListOfInstr *list, thTable * globalVarTable );
void interpretEval(tListOfInstr *list, thTable* localVarTable,thTable* globalVarTable);
void printInstType(int instructionType);