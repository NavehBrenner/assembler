#include "constants.h"

#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct symbolNode
{
  char name[MAXSYMBOLLEN];
  int address;
  int type;
  struct symbolNode *next;
} symbolNode;

#define CODE 0
#define DATA 1
#define EXTERN 2
#define ENTRY 3

symbolNode newSymbolNode(char *name);
symbolNode *emptySymbol(void);

int includesSymbol(symbolNode *table, symbolNode *symbol);

void insertSymbol(symbolNode *table, symbolNode *symbol);
void freeSymboltable(symbolNode *table);

#endif