#include "../@types/word.h"
#include "../@types/symbol.h"

#ifndef GENERALTRANSLATOR_H
#define GENERALTRANSLATOR_H

void translateLine(char *line, int *IC, int *DC, memory inst_m, memory data_m, symbolNode *table, symbolNode *codeSymbols, symbolNode *entries);

int isReg(tokenNode *tok);

word dataSource(tokenNode *tok, word *op);
word dataDestination(tokenNode *tok, word *op);
word registerSource(tokenNode *tok, word *op);
word registerDestination(tokenNode *tok, word *op);

#endif