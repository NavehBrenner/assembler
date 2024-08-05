#include "../@types/symbol.h"
#include "../@types/token.h"

#ifndef SYNTAX_H
#define SYNTAX_H

#define ERR_INVALID_TOKEN 0
#define ERR_REDEF 1
#define ERR_UNEXPECTED_TOKEN 2
#define ERR_INVALID_SRC 3
#define ERR_INVALID_DEST 4
#define ERR_MISSING_COMMA 5
#define ERR_EXPECTED_SYMBOL 6
#define ERR_EXPECTED_NUMBER 7
#define ERR_EXPECTED_STRING 8
#define ERR_UNDEFINED_SYMBOL 9
#define ERR_EXNT 10

int validateSymbols(int IC, symbolNode *lables, symbolNode *entries, symbolNode *codeSymbols, char *fileName);
int validateSyntax(char *line, symbolNode *lables, int lineNo, char *fileName);
int validLable(tokenNode *head, symbolNode *lables, int lineNo, char *fileName);
int validOp(tokenNode *head, int lineNo, char *fileName);
int validInst(tokenNode *head, symbolNode *lables, int lineNo, char *fileName);
int validDest(int opCode, tokenNode *dest);
int validSrc(int opCode, tokenNode *src);
int printErr(int errCode, int lineNo, char *fileName, char *tokStr);

#endif