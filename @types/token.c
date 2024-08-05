#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int getRegisterNum(char *str)
{
  const char c = str[strlen(str) - 1];
  return atoi(&c);
}

/// @brief extract the number value out of a token string
/// @param str token string to extract
/// @return number extracted
int extractNumber(char *str)
{
  if (isValidNumber(str))
    return atoi(str);
  return atoi(str + 1);
}

/// @brief check if string is a valid number
/// @param str
/// @return 1 for true, 0 for false
int isValidNumber(char *str)
{
  char *endPtr;
  strtol(str, &endPtr, 10);
  return *endPtr == '\0';
}

/// @brief check if string is a valid register
/// @param str
/// @return 1 for true, 0 for false
int isRegister(char *str)
{
  if (strlen(str) != 2)
    return 0;
  if (str[0] == 'r' && str[1] >= '0' && str[1] <= '7')
    return 1;
  return 0;
}

/// @brief check if string is a valid register pointer
/// @param str
/// @return 1 for true, 0 for false
int isRegisterPointer(char *str)
{
  return str[0] == '*' && isRegister(str + 1);
}

/// @brief check if string is a defined operator
/// @param str
/// @return 1 for true, 0 for false
int isOperator(char *str)
{
  char *oper[NO_OF_OPERATORS] = OPERATORS;
  for (int i = 0; i < NO_OF_OPERATORS; i++)
  {
    if (strcmp(str, oper[i]) == 0)
      return 1;
  }
  return 0;
}

/// @brief check if string is valid lable declaration
/// @param str
/// @return 1 for true, 0 for false
int isLable(char *str)
{
  int len = strlen(str);
  if (str[len - 1] != ':')
    return 0;
  char cpy[len];
  strcpy(cpy, str);
  cpy[len - 1] = '\0';
  return isSymbol(cpy);
}

/// @brief check if string is a valid instruction decleration
/// @param str
/// @return 1 for true, 0 for false
int isInstruction(char *str)
{
  char *instructions[NO_OF_INSTRUCTIONS] = INSTRUCTIONS_ARR;
  for (int i = 0; i < NO_OF_INSTRUCTIONS; i++)
    if (strcmp(str, instructions[i]) == 0)
      return 1;
  return 0;
}

/// @brief check if string is a valid symbol
/// @param str
/// @return 1 for true, 0 for false
int isSymbol(char *str)
{
  int len = strlen(str);
  if (len > 31 || !(isalpha(str[0])))
    return 0;

  for (int i = 1; i < len; i++)
  {
    if (!(isalnum(str[i]) || str[i] == '_'))
      return 0;
  }

  return 1;
}

/// @brief assign type to a string token or INVALID type if token doesnt match any type
/// @param tok
/// @return type assigned to token
int assignType(tokenNode *tok)
{
  char *str = tok->str;
  if (str[0] == ';')
    return (tok->type = COMMENT);
  if (isLable(str))
    return (tok->type = LABLE);
  if (strcmp(str, "macr") == 0)
    return (tok->type = MACRO);
  if (strcmp(str, "endmacr") == 0)
    return (tok->type = END_MACRO);
  if (str[0] == '#' && isValidNumber(str + 1))
    return (tok->type = IMMIDIATE_NUMBER);
  if (isValidNumber(str))
    return (tok->type = DATA_NUMBER);
  if (isRegister(str))
    return (tok->type = REGISTER);
  if (isRegisterPointer(str))
    return (tok->type = REGISTER_PTR);
  if (isOperator(str))
    return (tok->type = OPCODE);
  if (str[0] == '"' && str[strlen(str) - 1] == '"')
    return (tok->type = STRING);
  if (strcmp(str, ",") == 0)
    return (tok->type = COMMA);
  if (isInstruction(str))
    return (tok->type = INSTRUCTION);
  if (strcmp(str, "\n") == 0)
    return (tok->type = NEWLINE);
  if (isSymbol(str))
    return (tok->type = SYMBOL);
  return (tok->type = INVALID);
}

/// @brief free a tokenNode list
/// @param head pointer to list head
void freeTokenList(tokenNode *head)
{
  tokenNode *current = head;
  tokenNode *next;
  while (current != NULL)
  {
    next = current->next; // Save the next node
    free(current);        // Free the node
    current = next;       // Move to the next node
  }
}