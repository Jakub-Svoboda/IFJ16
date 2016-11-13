#include <stdio.h>
#include <stdlib.h>
#include "iList.h"
#include "htab.h"

#ifndef	INTERPRET_H
#define INTERPRET_H 


int runInterpret(tListOfInstr *list, thTable * globalVarTable );
void interpretEval(tListOfInstr *list, thTable* localVarTable,thTable* globalVarTable);
void printInstType(int instructionType);
#endif