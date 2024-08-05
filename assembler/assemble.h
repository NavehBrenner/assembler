#ifndef ASSEMBLER_H
#define ASSEMBLER_H

// #undef preAssemblerPath
#if defined(_WIN32) || defined(_WIN64)
#define preAssemblerPath ".\\preAssembler\\preAssembler.exe"
#define fileFolder ".\\tests\\"
#elif defined(__linux__)
#define fileFolder "./tests/"
#define preAssemblerPath "./preAssembler/preAssembler"
#else
#define preAssemblerPath "preAssembler/preAssembler.exe"
#endif

int assemble(char *fileName);
int preAssemble(char *filePath);

#endif