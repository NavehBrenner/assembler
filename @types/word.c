#include "word.h"
#include <string.h>
#include <stdio.h>

/// @brief get the machine word represented by a symbol out of a symbol list
/// @param lables symbol list to extract from
/// @param symbolName symbol to look for
/// @return 0 if symbol does not exist in list and the machine word represented by the symbol otherwise
word getWord(symbolNode *lables, char *symbolName)
{
  lables = lables->next;
  word out = 0x0;
  while (lables != NULL)
  {
    if (strcmp(lables->name, symbolName) == 0)
    {
      setValue(&out, lables->address);
      setARE(&out, lables->type == EXTERN ? E : R);
      return out;
    }
    lables = lables->next;
  }

  return out;
}

/// @brief update symbol addresses to staring loading point (100) and data symbols to be after code symbols
/// @param lables symbol list to update
/// @param IC last code line number
void updateSymbols(symbolNode *lables, int IC)
{
  lables = lables->next;
  while (lables != NULL)
  {
    if (lables->type != EXTERN) // extern symbols stay with address 0
      lables->address += lables->type == CODE ? 100 : 100 + IC;
    lables = lables->next;
  }
}

/// @brief complete a symbol adresses after all symbols parsed
/// @param inst_m intruction memory to complete adresses in
/// @param lables lables found
/// @param entries entry request to fill address in
/// @param codeSymbols code symbols to fill address in
void completeSymbols(memory inst_m, symbolNode *lables, symbolNode *entries, symbolNode *codeSymbols)
{
  codeSymbols = codeSymbols->next;
  while (codeSymbols != NULL)
  {
    inst_m[codeSymbols->address] = getWord(lables, codeSymbols->name);
    codeSymbols = codeSymbols->next;
  }

  entries = entries->next;
  while (entries != NULL)
  {
    entries->address = (getWord(lables, entries->name) >> 3);
    entries = entries->next;
  }
}

void setOpCode(word *word, int opcode)
{
  *word |= (opcode << 11);
}

void setSourceType(word *word, int type)
{
  *word |= (type << 7);
}

void setDestType(word *word, int type)
{
  *word |= (type << 3);
}

void setARE(word *word, int ARE)
{
  *word |= ARE;
}

void setValue(word *word, int value)
{
  *word |= (value << 3);
}

void setSource(word *word, int sourceR)
{
  *word |= (sourceR << 6);
}

void setDest(word *word, int destR)
{
  *word |= (destR << 3);
}
