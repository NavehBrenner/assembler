#include "../@types/token.h"
#include "../@types/word.h"
#include "../@types/symbol.h"
#include "instTrns.h"
#include <string.h>
#include <stdio.h>

/// @brief translte instruction line into machine code
/// @param head head of line
/// @param DC data counter
/// @param data_m data memory
/// @param symbolTable symbols table
/// @param entries entries requests list
void translateInstLine(tokenNode *head, int *DC, memory data_m, symbolNode *symbolTable, symbolNode *entries)
{
  if (strcmp(head->str, ".string") == 0)
  {
    // for .string add all characters of string into data momory
    head = head->next;
    for (int i = 1; i < strlen(head->str) - 1; i++)
      data_m[(*DC)++] = head->str[i];
    data_m[(*DC)++] = '\0'; // terminate with null char
  }

  else if (strcmp(head->str, ".data") == 0)
  {
    // for .data parse all numbers into data memory - each iteration jump 2 tokens to account for commas
    head = head->next;
    for (; head != NULL && head->next != NULL; head = head->next->next)
      data_m[(*DC)++] = extractNumber(head->str);
  }

  else if (strcmp(head->str, ".extern") == 0)
    addExternSymbol(symbolTable, head->next->str);

  else // .entry
    addEntryRequest(entries, head->next->str);
}

/// @brief add a new entry request into entries list
/// @param entries entries requests list
/// @param name name of new entry to add
void addEntryRequest(symbolNode *entries, char *name)
{
  symbolNode *symbol = emptySymbol();
  *symbol = newSymbolNode(name);
  symbol->type = ENTRY;
  insertSymbol(entries, symbol);
}

/// @brief add extern symbol to symbol table
/// @param symbolTable symbol table to add to
/// @param name name of symbol to create and add
void addExternSymbol(symbolNode *symbolTable, char *name)
{
  symbolNode *symbol = emptySymbol();
  *symbol = newSymbolNode(name);
  symbol->type = EXTERN;
  symbol->address = 0;
  insertSymbol(symbolTable, symbol);
}