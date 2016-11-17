#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "htab.h"
#include "garbage.h"

#ifndef	INTERPRET_H
#define INTERPRET_H 


int runInterpret(tListOfInstr *list);
thtabItem* interpretEval(tListOfInstr *list, thTable* localVarTable);
void printInstType(int instructionType);
#endif