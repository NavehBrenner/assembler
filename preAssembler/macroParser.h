#include "../lexer/lexer.h"
#include "macro.h"
#include <stdio.h>

#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H

#define ERR_MISSING_MCR_NAME 1
#define ERR_UNEXPECTED_TOKEN 2
#define ERR_INVALID_MCR_NAME 3
#define ERR_MCR_REDEF 4
#define ERR_INVALID_MCREND 5

int parseMacros(const char *);

void fileWriteMcr(FILE *to, macroNode *mcr);
int exitProtocol(tokenNode *lineHead, macroNode *mcrs, macroNode *lastMcr, int lineNo, int errorNo, tokenNode *badTok, const char *fileName, FILE *from, FILE *to);
void printMcrErr(int errNo, int lineNo, char *mcrName, const char *fileName);

#endif