#include "../@types/token.h"

#ifndef LEXER_H
#define LEXER_H

int isSep(char ch);
int parseSubstring(char *From, char *To, int startIndex);
tokenNode tokenizeStr(char *str);
tokenNode cpyToken(tokenNode *toCpy);
tokenNode *tokenize(char *line);
void terminateTokenList(tokenNode *head);

#endif