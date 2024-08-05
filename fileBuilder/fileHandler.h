#include <stdio.h>

#ifndef FILEHANLER_H
#define FILEHANLER_H

FILE *openDotasFile(const char *fileName);
FILE *createDotFile(const char *fileName, char *extention);
void removeDotFile(const char *fileName, char *extention);

#endif