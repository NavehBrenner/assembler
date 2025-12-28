#include "../@types/word.h"
#include "../@types/symbol.h"
#include "../machineCode/gnrlTrns.h"
#include "../fileBuilder/buildFiles.h"
#include "assemble.h"
#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "No files specified");
    exit(0);
  }

  // for each of the files specified
  for (int i = 1; i < argc; i++)
  {
    // look for the file in the ./tests folder Warning: Will NOT recognize file outside of ./tests folder
    char *filePath = (char *)malloc(strlen(fileFolder) + strlen(argv[i]) + 1);
    sprintf(filePath, "%s%s", fileFolder, argv[i]);
    if (!preAssemble(filePath)) // preAssemble file
      assemble(filePath);       // if preassemble was successful also try to assemble file
    free(filePath);
  }

  exit(0);
}

/// @brief call preAssembler with filePath
/// @param filePath path to file to preAssemble
/// @return 0 if preassemble was successful else 0
int preAssemble(char *filePath)
{
  char *command = (char *)malloc(strlen(preAssemblerPath) + strlen(filePath));
  sprintf(command, "%s %s", preAssemblerPath, filePath);
  // create preassemble command

  int status = system(command); // keep preassemble status to be able to free command
  free(command);
  return status;
}

/// @brief assemble file (do not inclued file extention)
/// @param fileName name of file to assmblt
/// @return 0 if assemble was successful else number of erros found
int assemble(char *fileName)
{
  FILE *f;
  char dotAmName[strlen(fileName) + strlen(".am")];
  sprintf(dotAmName, "%s.am", fileName);
  f = fopen(dotAmName, "r");
  if (!f)
  {
    printf("Error opening file (%s) - check file path\n", dotAmName);
    return 1;
  }

  int IC = 0, DC = 0;
  int lineNo = 1;
  int errors = 0;
  memory instruction_m, data_m;
  char line[81];
  symbolNode *table = emptySymbol();
  symbolNode *codeSymbols = emptySymbol();
  symbolNode *entries = emptySymbol();

  while (!(feof(f)) && (fgets(line, MAXLINELEN, f) != NULL))
  {
    errors += (validateSyntax(line, table, lineNo, fileName)) ? 0 : 1;
    translateLine(line, &IC, &DC, instruction_m, data_m, table, codeSymbols, entries);
    lineNo++;
  }

  updateSymbols(table, IC);
  completeSymbols(instruction_m, table, entries, codeSymbols);

  if (!validateSymbols(IC, table, entries, codeSymbols, fileName))
    errors++;

  if (!errors)
    buildFiles(fileName, codeSymbols, entries, instruction_m, data_m, IC, DC);

  freeSymboltable(table);
  freeSymboltable(codeSymbols);
  freeSymboltable(entries);
  return errors;
}
