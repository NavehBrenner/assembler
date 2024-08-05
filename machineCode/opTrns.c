#include "../@types/token.h"
#include "../@types/word.h"
#include "opTrns.h"
#include "gnrlTrns.h"

#include <string.h>
#include <stdio.h>

/// @brief match operator string to operator number code
/// @param tok opeartor token to match
/// @return op code, -1 if not an operator
int matchOperator(tokenNode *tok)
{
  char *oper[NO_OF_OPERATORS] = OPERATORS;
  for (int i = 0; i < NO_OF_OPERATORS; i++)
  {
    if (strcmp(tok->str, oper[i]) == 0)
      return i;
  }
  return -1;
}

/// @brief math opCode to number of operands operator takes
/// @param opcode number code of operator
/// @return number of operands for operator
int numOfOperands(int opcode)
{
  if (opcode <= 4)
    return 2;

  if (opcode <= 13)
    return 1;

  return 0;
}

/// @brief translate a operator line into machine code
/// @param head head to line
/// @param IC instruction counter
/// @param instucrion_m instrcution memory
/// @param codeSymbols code symbol references
void translateOpLine(tokenNode *head, int *IC, memory instucrion_m, symbolNode *codeSymbols)
{
  word opWord = 0x0;

  setOpCode(&opWord, matchOperator(head));
  setARE(&opWord, A);

  int opNo = numOfOperands(matchOperator(head));

  if (opNo == 0) // no operands - single word
    instucrion_m[(*IC)++] = opWord;

  else if (opNo == 1)
  {
    tokenNode *dest = head->next;

    if (dest->type == SYMBOL) // if destinaiton operand is a symbol add it to code symbol references
    {
      symbolNode *symbol = emptySymbol();
      *symbol = newSymbolNode(dest->str);
      symbol->address = (*IC) + 1;
      insertSymbol(codeSymbols, symbol);
    }

    // create the matching destinaiton word and add to instruction memory
    instucrion_m[(*IC) + 1] = isReg(dest) ? registerDestination(dest, &opWord) : dataDestination(dest, &opWord);
    instucrion_m[*IC] = opWord;
    (*IC) += 2;
  }

  // opNo == 2
  else
    parseTwoOperands(head, &opWord, IC, instucrion_m, codeSymbols);
}

/// @brief parse two operands operator
/// @param head head of line
/// @param opWord operator word
/// @param IC instruction counter
/// @param instruction_m instruction memory
/// @param codeSymbols code symbol references
void parseTwoOperands(tokenNode *head, word *opWord, int *IC, memory instruction_m, symbolNode *codeSymbols)
{
  tokenNode *src = head->next;
  tokenNode *dest = head->next->next->next;

  if (isReg(src) && isReg(dest))
  {
    // special case of two register operands
    doubleRegister(src, dest, opWord, IC, instruction_m);
    return;
  }

  // if any of source ot destination word are symbol add them to symbol table
  if (src->type == SYMBOL)
  {
    symbolNode *symbol = emptySymbol();
    *symbol = newSymbolNode(src->str);
    symbol->address = (*IC) + 1;
    insertSymbol(codeSymbols, symbol);
  }
  if (dest->type == SYMBOL)
  {
    symbolNode *symbol = emptySymbol();
    *symbol = newSymbolNode(dest->str);
    symbol->address = (*IC) + 2;
    insertSymbol(codeSymbols, symbol);
  }

  // create source and destination words and add to instruction memory
  destination(dest, opWord, (*IC) + 2, instruction_m);
  source(src, opWord, *IC, instruction_m);
  (*IC) += 3;
}

/// @brief special two register operands parser
/// @param src source opernad
/// @param dest destination operand
/// @param opWord operator word
/// @param IC instruction counter
/// @param instruction_m intruction memory
void doubleRegister(tokenNode *src, tokenNode *dest, word *opWord, int *IC, memory instruction_m)
{
  // put both the source and destination in a single word add insert into instruction memory
  instruction_m[(*IC) + 1] = (registerSource(src, opWord) | registerDestination(dest, opWord));
  instruction_m[(*IC)] = *opWord;
  (*IC) += 2;
}

/// @brief add a source word to memory
/// @param src source token
/// @param opWord operator word
/// @param IC instruction counter
/// @param instruction_m instruction memory
void source(tokenNode *src, word *opWord, int IC, memory instruction_m)
{
  // create matchig source and insert it and op word
  instruction_m[IC + 1] = isReg(src) ? registerSource(src, opWord) : dataSource(src, opWord);
  instruction_m[IC] = *opWord;
}

/// @brief add a destination word to memory
/// @param dest destination token
/// @param opWord opeartor word
/// @param IC instruction counter
/// @param instruction_m instruction memory
void destination(tokenNode *dest, word *opWord, int IC, memory instruction_m)
{
  instruction_m[IC] = isReg(dest) ? registerDestination(dest, opWord) : dataDestination(dest, opWord);
}
