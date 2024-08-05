#include "constants.h"

#ifndef TOKEN_H
#define TOKEN_H

typedef struct tokenNode
{
  int type;
  char str[MAXSYMBOLLEN];
  struct tokenNode *next;
} tokenNode;

int getRegisterNum(char *str);
int extractNumber(char *str);
void freeTokenList(tokenNode *head);
int isValidNumber(char *str);
int isRegister(char *str);
int isRegisterPointer(char *str);
int isOperator(char *str);
int isLable(char *str);
int isInstruction(char *str);
int isSymbol(char *str);
int assignType(tokenNode *tok);

enum constants
{
  INVALID,
  MACRO,
  END_MACRO,
  COMMENT,
  STRING,
  DATA_NUMBER,
  IMMIDIATE_NUMBER,
  OPCODE,
  REGISTER,
  REGISTER_PTR,
  SYMBOL,
  INSTRUCTION,
  COMMA,
  LABLE,
  NEWLINE,
};

#define MAX_LABLE_LEN 31
#define OPERATORS \
  {               \
      "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"}

#define INSTRUCTIONS_ARR \
  {                      \
      ".data", ".string", ".extern", ".entry"}
#define NO_OF_INSTRUCTIONS 4
#define NO_OF_OPERATORS 16
#endif