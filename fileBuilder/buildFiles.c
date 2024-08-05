#include "../@types/symbol.h"
#include "../@types/word.h"
#include "fileHandler.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/// @brief build object file out of machine memory
/// @param fileName name for .ob file
/// @param inst_m instruction memory
/// @param data_m data memory
/// @param IC instruction counter
/// @param DC data counter
void buildObFile(char *fileName, memory inst_m, memory data_m, int IC, int DC)
{
  FILE *ob = createDotFile(fileName, "ob");

  fprintf(ob, "%5d %d\n", IC, DC);

  for (int i = 0; i < IC + DC; i++) // loop over the memeory
  {
    unsigned int mask = 0x7fff; // mask to extract only first 15 bits
    if (i < IC)                 // decide witch memory to print from (first inst_m then data_m)
      fprintf(ob, "%04d\t%05o\n", i + 100, mask & inst_m[i]);
    else
      fprintf(ob, "%04d\t%05o\n", i + 100, mask & data_m[i - IC]);
  }

  fclose(ob);
}

/// @brief build .ext file from symbols found in code
/// @param fileName name of .ext file to build
/// @param codeSymbols  all symbols found in code
/// @param inst_m instruction memory
void buildExtFile(char *fileName, symbolNode *codeSymbols, memory inst_m)
{
  FILE *ext = createDotFile(fileName, "ext");

  codeSymbols = codeSymbols->next;
  for (; codeSymbols->next != NULL; codeSymbols = codeSymbols->next)
  { // find all extern symbol references in code and print them into .ext file
    if (inst_m[codeSymbols->address] & E)
      fprintf(ext, "%s\t%04d\n", codeSymbols->name, codeSymbols->address + 100);
  }

  fseek(ext, 0, SEEK_END);
  int size = ftell(ext);

  fclose(ext);

  if (size == 0)
    removeDotFile(fileName, "ext");
}

/// @brief build .ent file
/// @param fileName .ent file name to create
/// @param entries entries requests
void buildEntFile(char *fileName, symbolNode *entries)
{
  FILE *ent = createDotFile(fileName, "ent");

  entries = entries->next;
  for (; entries != NULL; entries = entries->next)
    fprintf(ent, "%s\t%04d\n", entries->name, entries->address);

  fseek(ent, 0, SEEK_END);
  int size = ftell(ent);

  fclose(ent);

  if (size == 0)
    removeDotFile(fileName, "ent");
}

/// @brief build output files after assembler execution - empty output files will then be deleted
/// @param fileName name of file to create
/// @param codeSymbols symbol references in code
/// @param entries entries requests
/// @param inst_m instruction memory
/// @param data_m data memory
/// @param IC instruction counter
/// @param DC data counter
void buildFiles(char *fileName, symbolNode *codeSymbols, symbolNode *entries, memory inst_m, memory data_m, int IC, int DC)
{
  buildObFile(fileName, inst_m, data_m, IC, DC);
  buildExtFile(fileName, codeSymbols, inst_m);
  buildEntFile(fileName, entries);
}