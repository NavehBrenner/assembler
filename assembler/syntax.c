#include "../@types/token.h"
#include "../@types/symbol.h"
#include "../lexer/lexer.h"
#include "../machineCode/opTrns.h"
#include "syntax.h"

#include <stdio.h>
#include <string.h>

/// @brief check for valid symbols, no symbols wihtout declerations and not symbols marked both as entry and extern
/// @param lables lables found during assemble
/// @param entries entry requests
/// @param codeSymbols symbols found in code
/// @param fileName name of file currently assmbled
/// @return 1 if valid else 0
int validateSymbols(int IC, symbolNode *lables, symbolNode *entries, symbolNode *codeSymbols, char *fileName)
{
  int errors = 0;
  codeSymbols = codeSymbols->next;
  for (; codeSymbols != NULL; codeSymbols = codeSymbols->next)
  {
    if (!includesSymbol(lables, codeSymbols))
      errors += !printErr(ERR_UNDEFINED_SYMBOL, codeSymbols->address, fileName, codeSymbols->name);
  }

  entries = entries->next;
  for (; entries != NULL; entries = entries->next)
  {
    if (getWord(lables, entries->name) & E)
    {
      int lineNo = entries->address + IC - 1;
      errors += !printErr(ERR_EXNT, lineNo, fileName, entries->name);
    }
  }
  return !errors;
}

/// @brief check syntax of line
/// @param line
/// @param lables lables found so far
/// @param lineNo line number
/// @param fileName
/// @return 1 if line syntax is valid else 0
int validateSyntax(char *line, symbolNode *lables, int lineNo, char *fileName)
{
  tokenNode *head = tokenize(line);
  if (head->type == INVALID) // if a invlid token exists in line
    return printErr(ERR_INVALID_TOKEN, lineNo, fileName, head->str);

  if (head->type == LABLE) // lable decleration
  {
    if (!validLable(head, lables, lineNo, fileName))
      return 0;
    head = head->next;
  }

  if (head->type == OPCODE) // operator
    return validOp(head, lineNo, fileName);

  if (head->type == INSTRUCTION) // instrunction
    return validInst(head, lables, lineNo, fileName);

  return printErr(ERR_UNEXPECTED_TOKEN, lineNo, fileName, head->str); // unexpected token after line
}

/// @brief check that a lable decleration is valid
/// @param head head of line of lable dec
/// @param lables current lables found
/// @param lineNo line number
/// @param fileName cur file name
/// @return 1 if valid else 0
int validLable(tokenNode *head, symbolNode *lables, int lineNo, char *fileName)
{
  symbolNode symbol = newSymbolNode(head->str); // create a symbol from current lable
  if (includesSymbol(lables, &symbol))          // check for redefinition of symbol
    return printErr(ERR_REDEF, lineNo, fileName, symbol.name);

  if (strcmp(head->next->str, ".entry") == 0) // no meaning entry lable warning
    printf("%s.as:%d: Warning - Lable defintion of (%s) has no meaning\n", fileName, lineNo, symbol.name);

  return 1;
}

/// @brief validate syntax of a operation line
/// @param head head of op line
/// @param lineNo line number
/// @param fileName cur file name
/// @return 1 if valid else 0
int validOp(tokenNode *head, int lineNo, char *fileName)
{
  int opCode = matchOperator(head);
  int noOfOp = numOfOperands(opCode);

  head = head->next;
  if (noOfOp == 1 && !validDest(opCode, head)) // one operand - only destination
    return printErr(ERR_INVALID_DEST, lineNo, fileName, head->str);

  if (noOfOp == 2) // two operands
  {
    if (!validSrc(opCode, head)) // check for valid source
      return printErr(ERR_INVALID_SRC, lineNo, fileName, head->str);

    head = head->next;
    if (head->type != COMMA) // check for seperatin comma
      return printErr(ERR_MISSING_COMMA, lineNo, fileName, head->str);

    head = head->next;
    if (!validDest(opCode, head)) // checl for valid destination
      return printErr(ERR_INVALID_DEST, lineNo, fileName, head->str);
  }

  if (head->next != NULL && head->next->type != NEWLINE) // if any more tokens exist after also checks for 0 operands case
    return printErr(ERR_UNEXPECTED_TOKEN, lineNo, fileName, head->str);
  return 1; // return 1 for valid
}

/// @brief check for valid instruction line syntx
/// @param head head of line to check
/// @param lables lablse found so far
/// @param lineNo line number
/// @param fileName cur file name
/// @return 1 if valid else 0
int validInst(tokenNode *head, symbolNode *lables, int lineNo, char *fileName)
{
  // .entry && .extern
  if (strcmp(head->str, ".entry") == 0 || strcmp(head->str, ".extern") == 0)
  {
    head = head->next; // check for a symbol after .entry or .extern
    if (head->type != SYMBOL)
      return printErr(ERR_EXPECTED_SYMBOL, lineNo, fileName, head->str);

    // in extern check that lable decleration is valid (entries check will be done later)
    if (strcmp(head->str, ".extern") == 0 && !validLable(head, lables, lineNo, fileName))
      return 0;
    head = head->next;
  }

  // .data
  if (strcmp(head->str, ".data") == 0)
  {
    head = head->next;
    for (int numFlag = 0x1; head->next != NULL; head = head->next, numFlag ^= 1)
    { // check for altering numbers and commas
      if (head->type == NEWLINE)
        return numFlag ? printErr(ERR_EXPECTED_NUMBER, lineNo, fileName, head->str) : 1;
      if (numFlag && head->type != DATA_NUMBER)
        return printErr(ERR_EXPECTED_NUMBER, lineNo, fileName, head->str);
      if (!numFlag && head->type != COMMA)
        return printErr(ERR_MISSING_COMMA, lineNo, fileName, head->next->str);
    }
  }

  // .string
  if (strcmp(head->str, ".string") == 0)
  {
    head = head->next;
    if (head->type != STRING) // check for valid string
      return printErr(ERR_EXPECTED_STRING, lineNo, fileName, head->str);
  }

  // check for no tokens after end of instruction
  if (head->next != NULL && head->next->type != NEWLINE)
    printErr(ERR_UNEXPECTED_TOKEN, lineNo, fileName, head->next->str);
  return 1; // return 1 for valid
}

/// @brief check for valid destination type
/// @param opCode operator code
/// @param dest detination operand for operator
/// @return 1 if valie else 0
int validDest(int opCode, tokenNode *dest)
{
  int type = dest->type;
  if (opCode == CMP || opCode == PRN) // 0, 1, 2, 3
    return type == IMMIDIATE_NUMBER || type == SYMBOL || type == REGISTER || type == REGISTER_PTR;

  if ((opCode >= MOV && opCode <= DEC) || opCode == RED) // 1, 2, 3
    return type == SYMBOL || type == REGISTER || type == REGISTER_PTR;

  // 1, 2
  return type == SYMBOL || type == REGISTER_PTR;
}

/// @brief check for valid source type
/// @param opCode operator code
/// @param src source operand for operator
/// @return 1 if valid else 0
int validSrc(int opCode, tokenNode *src)
{
  int type = src->type;
  if (MOV <= opCode && opCode <= SUB) // 1, 2, 3, 4
    return type == IMMIDIATE_NUMBER || type == SYMBOL || type == REGISTER || type == REGISTER_PTR;
  return type == SYMBOL; // 1
}

/// @brief print error (returns 0)
/// @param errCode
/// @param lineNo
/// @param fileName
/// @param tokStr
/// @return 0
int printErr(int errCode, int lineNo, char *fileName, char *tokStr)
{
  fprintf(stderr, "%s.am:%d: Error - ", fileName, lineNo);
  if (errCode == ERR_INVALID_TOKEN)
    fprintf(stderr, "Invalid token '%s'\n", tokStr);
  if (errCode == ERR_REDEF)
    fprintf(stderr, "Redefinition of symbol (%s)\n", tokStr);
  if (errCode == ERR_UNEXPECTED_TOKEN)
    fprintf(stderr, "Unexpected token (%s)\n", tokStr);
  if (errCode == ERR_INVALID_SRC)
    fprintf(stderr, "Invalid source (%s)\n", tokStr);
  if (errCode == ERR_INVALID_DEST)
    fprintf(stderr, "Invalid destination (%s)\n", tokStr);
  if (errCode == ERR_MISSING_COMMA)
    fprintf(stderr, "Missing comma before token (%s)\n", tokStr);
  if (errCode == ERR_EXPECTED_SYMBOL)
    fprintf(stderr, "Expected symbol - got (%s) insead\n", tokStr);
  if (errCode == ERR_EXPECTED_NUMBER)
    fprintf(stderr, "Expected number - got (%s) instead\n", tokStr);
  if (errCode == ERR_EXPECTED_STRING)
    fprintf(stderr, "Expected string - got (%s) instead\n", tokStr);
  if (errCode == ERR_UNDEFINED_SYMBOL)
    fprintf(stderr, "No definition found for symbol (%s)\n", tokStr);
  if (errCode == ERR_EXNT)
    fprintf(stderr, ".entry definition for .extern lable (%s)\n", tokStr);
  return 0;
}