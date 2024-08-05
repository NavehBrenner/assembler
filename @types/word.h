#include "token.h"
#include "symbol.h"

#ifndef WORD_H
#define WORD_H

#define IMMIDIATE 0x1
#define DIRECT 0x2
#define INDIRECT_R 0x4
#define DIRECT_R 0x8

#define A 0x4
#define R 0x2
#define E 0x1

#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define LEA 4
#define CLR 5
#define NOT 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

typedef unsigned int word;
typedef word memory[4096];

word getWord(symbolNode *lables, char *symbolName);
void updateSymbols(symbolNode *lables, int IC);
void completeSymbols(memory inst_m, symbolNode *lables, symbolNode *entries, symbolNode *codeSymbols);

void setOpCode(word *word, int opcode);
void setSourceType(word *word, int type);
void setDestType(word *word, int type);
void setARE(word *word, int ARE);
void setValue(word *word, int value);
void setSource(word *word, int sourceR);
void setDest(word *word, int destR);

#endif
