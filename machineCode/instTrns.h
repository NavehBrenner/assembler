#include "../@types/symbol.h"

#ifndef INSTTRNS_H
#define INSTTRNS_H

void translateInstLine(tokenNode *head, int *DC, memory data_m, symbolNode *symbolTable, symbolNode *entries);

void addExternSymbol(symbolNode *table, char *name);

void addEntryRequest(symbolNode *entries, char *name);
#endif