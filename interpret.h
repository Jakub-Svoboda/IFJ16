#include <stdio.h>
#include <stdlib.h>
#include "iList.h"
#include "htab.h"
#include "garbage.h"

#ifndef	INTERPRET_H
#define INTERPRET_H 


int runInterpret(tListOfInstr *list);
void interpretEval(tListOfInstr *list, thTable* localVarTable);
void printInstType(int instructionType);
#endif