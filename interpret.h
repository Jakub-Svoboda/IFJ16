#include <stdio.h>
#include <stdlib.h>
#include "iList.h"

int runInterpret(tListOfInstr *list, thTable * globalVarTable,thTable * functionTable );
void interpretEval(tListOfInstr *list);
void printInstType(int instructionType);