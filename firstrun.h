/*
* Project: Implementace interpretu imperativniho jazyka IFJ16
* File:    firstrun.h
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
#include <string.h>
#ifndef	FIRSTRUN_H
#define FIRSTRUN_H 

int firstRun(FILE * f);
void functionInsert(Token * tokenName, Token * tokenType,Token * tokenClass);
void globalVarInsert(Token * tokenName, Token * tokenType,Token * tokenClass);
#endif