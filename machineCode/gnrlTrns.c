#include "../@types/token.h"
#include "../lexer/lexer.h"
#include "../@types/symbol.h"
#include "../@types/word.h"
#include "gnrlTrns.h"
#include "instTrns.h"
#include "opTrns.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/// @brief translate line into machine code
/// @param line line to translte
/// @param IC current instruction counter
/// @param DC current data counter
/// @param inst_m instruction_memory
/// @param data_m data_memory
/// @param table currnet lable table
/// @param codeSymbols symbol code reference list
/// @param entries entries request list
void translateLine(char *line, int *IC, int *DC, memory inst_m, memory data_m, symbolNode *table, symbolNode *codeSymbols, symbolNode *entries)
{
  tokenNode *tokenLine = tokenize(line);
  tokenNode *head = tokenLine;

  if (head->type == LABLE) // add new lable to table and proceed to translte rest of line
  {
    symbolNode *symbol = emptySymbol();
    *symbol = newSymbolNode(head->str);
    head = head->next;
    symbol->type = (head->type == INSTRUCTION) ? DATA : CODE;
    symbol->address = (head->type == INSTRUCTION) ? *DC : *IC;
    insertSymbol(table, symbol);
  }

  if (head->type == INSTRUCTION)
    translateInstLine(head, DC, data_m, table, entries);

  if (head->type == OPCODE)
    translateOpLine(head, IC, inst_m, codeSymbols);

  freeTokenList(tokenLine);
}

/// @brief check if token is of type register or register pointer
/// @param tok
/// @return 1 if true else false
int isReg(tokenNode *tok)
{
  return (tok->type == REGISTER || tok->type == REGISTER_PTR);
}

/// @brief create a source data (number or symbol) word
/// @param tok token to create word from
/// @param op operator word to update
/// @return source word
word dataSource(tokenNode *tok, word *op)
{
  word source = 0x0;
  setValue(&source, tok->type == IMMIDIATE_NUMBER ? extractNumber(tok->str) : 0x0);
  setARE(&source, tok->type == IMMIDIATE_NUMBER ? A : 0x0);
  // update source type in operator word
  setSourceType(op, tok->type == IMMIDIATE_NUMBER ? IMMIDIATE : DIRECT);
  return source;
}

/// @brief create a destination data (number or symbol) word
/// @param tok token to create destination from
/// @param op operator word to update
/// @return destination word
word dataDestination(tokenNode *tok, word *op)
{
  word destination = 0x0;
  setValue(&destination, tok->type == IMMIDIATE_NUMBER ? extractNumber(tok->str) : 0x0);
  setARE(&destination, tok->type == IMMIDIATE_NUMBER ? A : 0x0);
  // update operator word destination type
  setDestType(op, tok->type == IMMIDIATE_NUMBER ? IMMIDIATE : DIRECT);
  return destination;
}

/// @brief create a source register word
/// @param tok token to create source from
/// @param op opeartor word to update
/// @return source word
word registerSource(tokenNode *tok, word *op)
{
  word source = 0x0;
  setSource(&source, getRegisterNum(tok->str));
  setARE(&source, A);
  // update operator word source type
  setSourceType(op, tok->type == REGISTER ? DIRECT_R : INDIRECT_R);
  return source;
}

/// @brief create a destination register word
/// @param tok token to create word from
/// @param op operator word to update
/// @return destination word
word registerDestination(tokenNode *tok, word *op)
{
  word destination = 0x0;
  setDest(&destination, getRegisterNum(tok->str));
  setARE(&destination, A);
  // update operator destination type
  setDestType(op, tok->type == REGISTER ? DIRECT_R : INDIRECT_R);
  return destination;
}
