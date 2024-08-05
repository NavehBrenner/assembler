#include "../@types/symbol.h"
#include "../@types/word.h"

#ifndef BUILDFILES_H
#define BUILDFILES_H

void buildFiles(char *fileName, symbolNode *codeSymbols, symbolNode *entries, memory inst_m, memory data_m, int IC, int DC);
void buildObFile(char *fileName, memory inst_m, memory data_m, int IC, int DC);
void buildExtFile(char *fileName, symbolNode *codeSymbols, memory inst_m);
void buildEntFile(char *fileName, symbolNode *entries);

#endif