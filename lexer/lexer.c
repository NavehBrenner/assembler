#include "lexer.h"
#include "../@types/token.h"
#include "../@types/constants.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief create a new token from string (malloc - use free)
/// @param str
/// @return pointer to new token
tokenNode tokenizeStr(char *str)
{
  tokenNode token;
  strcpy(token.str, str);
  assignType(&token);
  token.next = NULL;
  return token;
}

tokenNode cpyToken(tokenNode *toCpy)
{
  return tokenizeStr(toCpy->str);
}
/// @brief check for seperating char other then whitespace
/// @param ch
/// @return 1 for true, 0 for false
int isSep(char ch)
{
  if (ch == ',' || ch == ';' || ch == '\n')
    return 1;
  return 0;
}

/// @brief parse a substring from line starting at index startIndex
/// @param line
/// @param to parse to
/// @param startIndex
/// @return index in line where parsing stopped
int parseSubstring(char *line, char *to, int startIndex)
{
  int ch;
  int toInd = 0, i = startIndex;

  for (; line[i] == ' ' || line[i] == '\t' || line[i] == '\r'; i++)
    ; // remove any leading whitespaces

  for (; (ch = line[i]) != '\0'; i++)
  {
    // parse a "string" in the string line
    if (ch == '"')
    {
      to[toInd++] = ch;
      for (; (ch = line[++i]) != '\0' && ch != '"';)
        to[toInd++] = ch;
      break;
    }

    if (isSep(ch) || ch == ' ' || ch == '\t')
      break;

    to[toInd++] = ch;
  }

  if (ch == ':' || ch == '"' || !toInd)
  {
    to[toInd++] = ch;
    i++;
  }

  to[toInd] = '\0';    // terminate output str
#if defined(__linux__) // handle the different newline in windows and lunix
  if (to[toInd - 1] == '\r')
    to[toInd - 1] = '\0';
#endif

  return i;
}

/// @brief create a token list from an input line (malloc - use free)
/// @param line
/// @return pointer to head of list
tokenNode *tokenize(char *line)
{
  tokenNode *tokens = (tokenNode *)malloc(sizeof(tokenNode));
  tokenNode *tokenCpy = tokens;
  int curInd = 0;
  char curStr[MAXLINELEN];

  while (line[curInd] != '\0')
  {
    curInd = parseSubstring(line, curStr, curInd);
    *tokenCpy = tokenizeStr(curStr);

    for (; line[curInd] == ' ' || line[curInd] == '\t' || line[curInd] == '\r'; curInd++)
      ; // remove any unwanted trailing characters

    tokenCpy->next = line[curInd] != '\0' ? (tokenNode *)malloc(sizeof(tokenNode)) : NULL;
    tokenCpy = tokenCpy->next;
  }

  terminateTokenList(tokens);
  return tokens;
}

/// @brief termainte a token list with a newline token
/// @param head token list to terminate
void terminateTokenList(tokenNode *head)
{
  while (head != NULL && head->next != NULL && head->type != NEWLINE)
    head = head->next;

  if (head == NULL)
    return;

  if (head->type != NEWLINE)
  {
    head->next = (tokenNode *)malloc(sizeof(tokenNode));
    head = head->next;
    *head = tokenizeStr("\n");
  }

  head->next = NULL;
}