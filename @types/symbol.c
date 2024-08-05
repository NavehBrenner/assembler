#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// in many places in the code a statemet such as (symbolNode type) = (symbolNode type).next
// this is due to the reason that when using pointers and malloc it is much easier to insert a node into an existing list rather than having to check if the list exists in the first place then init it and otherwise insert. that means that in all symbolNode list the first item has no meaning thus moving to the next one

/// @brief create a default symbol node with name "lable"
/// @param lable name of symbol to be created
/// @return the symbolNode created
symbolNode newSymbolNode(char *lable)
{
  symbolNode symbol;
  int last = lable[strlen(lable) - 1] == ':' ? -1 : 0;
  strncpy(symbol.name, lable, strlen(lable) + last);
  symbol.name[strlen(lable) + last] = '\0';
  symbol.next = NULL;
  symbol.address = -1;
  symbol.type = -1;
  return symbol;
}

/// @brief init a new empty symbolNode (use malloc - call free)
/// @return pointer to symbolNode created
symbolNode *emptySymbol()
{
  symbolNode *s = (symbolNode *)malloc(sizeof(symbolNode));
  s->next = NULL;
  s->address = -1;
  s->type = -1;
  return s;
}

/// @brief insert a symbol into a symbol list
/// @param table list to insert to
/// @param symbol synbol to insert
void insertSymbol(symbolNode *table, symbolNode *symbol)
{
  while (table->next != NULL)
    table = table->next;

  table->next = symbol;
}

/// @brief check if a symbol list inclueds a symbol with same name
/// @param table list to check if includes
/// @param symbol symbol to check if included
/// @return 1 if a similar name symbol exist else 0
int includesSymbol(symbolNode *table, symbolNode *symbol)
{
  table = table->next;
  for (; table != NULL; table = table->next)
  {
    if (strcmp(table->name, symbol->name) == 0)
      return 1;
  }

  return 0;
}

/// @brief free malloc use of a simbolNode list
/// @param table list to free
void freeSymboltable(symbolNode *table)
{
  symbolNode *current = table;
  symbolNode *next;
  while (current->next != NULL)
  {
    next = current->next; // Save the next node
    free(current);        // Free the node
    current = next;       // Move to the next node
  }
}