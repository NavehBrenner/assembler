#include "fileHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief open a ".as" file
/// @param fileName filename (wihtout .as extention)
/// @return pointer to file
FILE *openDotasFile(const char *fileName)
{
  char *fileAsName = (char *)malloc(strlen(fileName) + strlen(".as") + 1);
  sprintf(fileAsName, "%s.as", fileName); // add the .as extention

  FILE *f = fopen(fileAsName, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Error opening file (%s) - cannot proceed - check file path\n", fileAsName);
    exit(1);
  }

  free(fileAsName); // free fileName after file opened

  return f;
}

/// @brief create a ".am" file
/// @param fileName name of file to create
/// @return pointer to file created
FILE *createDotFile(const char *fileName, char *extention)
{
  char *fileAmName = (char *)malloc(strlen(fileName) + strlen(extention) + 2);
  sprintf(fileAmName, "%s.%s", fileName, extention);

  FILE *f = fopen(fileAmName, "w");

  if (f == NULL)
  {
    fprintf(stderr, "Error creating file (%s) - cannot proceed - check file path\n", fileAmName);
    exit(1);
  }

  free(fileAmName);

  return f;
}

/// @brief delete a .am file
/// @param fileName name of file to delete (not including the ".am" extention)
void removeDotFile(const char *fileName, char *extention)
{
  char *fileAmName = (char *)malloc(strlen(fileName) + strlen(extention) + 2);
  sprintf(fileAmName, "%s.%s", fileName, extention);

  if (remove(fileAmName))
    fprintf(stderr, "Error removing file (%s) - cannot proceed - check file path\n", fileAmName);

  free(fileAmName);
}