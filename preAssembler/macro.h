#include "../@types/token.h"

#ifndef MACRO_H
#define MACRO_H

typedef struct macroNode
{
  char *name;
  tokenNode *code;
  struct macroNode *next;
} macroNode;

macroNode newMcrNode(char *name);
macroNode *newMcrList(void);
macroNode *emptyMcr(void);

void freeMcrList(macroNode *head);

macroNode *mcrDefined(macroNode *macros, char *name);
int validMcrEnd(tokenNode *head);
int validMcrDec(tokenNode *curLineHead, macroNode *macros);

void endMacro(int *flag, tokenNode *codePrev, macroNode *curMcr);
void addCode(tokenNode *addTo, tokenNode *code);
#endif