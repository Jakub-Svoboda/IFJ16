/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    interpret.h
* Date:    31.10.2016
* Team:    Michael Schmid      xschmi08
*          Jakub Svoboda        xsvobo0z
*          Jan Grossmann        xgross09
*          Matyas Sladek        xslade21
*          Katerina Smajzrova   xsmajz00
* Variant: b/1/II
*/

#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "ial.h"
#include "garbage.h"

#ifndef	INTERPRET_H
#define INTERPRET_H 


int runInterpret(tListOfInstr *list);
thtabItem* interpretEval(tListOfInstr *list, thTable* localVarTable);
void printInstType(int instructionType);
#endif