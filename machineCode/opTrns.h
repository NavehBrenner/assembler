#include "../@types/word.h"
#include "../@types/symbol.h"
#include "../@types/token.h"

#ifndef OPTRANSLATOR_H
#define OPTRANSLATOR_H

int matchOperator(tokenNode *tok);
int numOfOperands(int opcode);
void translateOpLine(tokenNode *head, int *IC, memory instucrion_m, symbolNode *codeSymbols);

void parseTwoOperands(tokenNode *head, word *opWord, int *IC, memory inst_m, symbolNode *codeSymbols);
void doubleRegister(tokenNode *src, tokenNode *dest, word *opWord, int *IC, memory inst_m);
void source(tokenNode *src, word *opWord, int IC, memory inst_m);
void destination(tokenNode *dest, word *opWord, int IC, memory inst_m);

#endif