#include "macroParser.h"
#include "macro.h"
#include "../fileBuilder/fileHandler.h"
#include "../@types/constants.h"
#include "../@types/token.h"
#include "../lexer/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// call the preAssembler, becuase in the macro phase a error means stop running its easear to use exit rather then nested returns, for that means we will have to call the preAssembler from the command line in the assembler so assembler wont stop execution
int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Error Invoking macroParser (%s)\n", argv[0]);
    exit(1);
  }

  parseMacros(argv[1]);
}

// we can assume no nested macros and every macro has a macrend
// thus only need to check for valid macro decleration
int parseMacros(const char *fileName)
{
  FILE *from = openDotasFile(fileName), *to = createDotFile(fileName, "am");
  char line[MAXLINELEN + 1];
  int lineNo = 1;
  macroNode *mcrHead = emptyMcr();
  macroNode *macros = mcrHead, *macrosPrev = NULL;
  tokenNode *code, *codePrev;
  tokenNode *curTok, *head;
  int mcrflag = 0;

  while (!(feof(from)) && (fgets(line, MAXLINELEN, from) != NULL))
  {
    curTok = (head = tokenize(line));

    if (head->type == COMMENT || head->type == NEWLINE) // ignore comments and empty lines
      continue;

    for (; curTok != NULL; curTok = curTok->next) // iterate over the input line tokens
    {
      if (curTok->type == END_MACRO)
      { // if found a macro end decleration
        endMacro(&mcrflag, codePrev, (macrosPrev = macros));
        macros = macros->next;

        if (!(validMcrEnd(head))) // if decleration was invalid exit program
          exitProtocol(head, mcrHead, macrosPrev, lineNo, ERR_INVALID_MCREND, curTok, fileName, from, to);
      }

      if (mcrflag) // if the macro flag is on - should write line into macro
      {
        addCode((codePrev = code), curTok);
        code = code->next;
      }

      if (!mcrflag) // if the macro flag is off - should write line into .am file
      {
        macroNode *tmpMcr;
        if ((tmpMcr = mcrDefined(mcrHead, curTok->str))) // if curTok is a macro expand it
          fileWriteMcr(to, tmpMcr);

        else if (head->type != MACRO && head->type != END_MACRO) // check that macro flag wasnt flipped on this line (prevents from writing into .am file "macr" & "macrend")
        {
          fprintf(to, "%s", curTok->str);
          if ((curTok->type != NEWLINE))
            fprintf(to, " ");
        }
      }

      if (curTok->type == MACRO) // found macro decleration
      {
        curTok = curTok->next; // proceed to next token (expected to be macro name)
        int error;
        if ((error = validMcrDec(head, mcrHead))) // if macro decleration is invalid exit program
          exitProtocol(head, mcrHead, macrosPrev, lineNo, error, curTok, fileName, from, to);

        // valid macro decleration
        mcrflag = 1;
        *macros = newMcrNode(curTok->str);
        code = macros->code;
        curTok = curTok->next; // valid macro decleration will always end in a new line, proceed to next token so newline wont be added to start of macro code
      }
    }

    lineNo++;
    freeTokenList(head);
  }

  freeMcrList(mcrHead);
  exit(0);
}

/// @brief writes (expands) a macro to a file (appends the text)
/// @param to pointer to file to write to
/// @param mcr macro to write
void fileWriteMcr(FILE *to, macroNode *mcr)
{
  tokenNode *code = mcr->code;
  while (code->next != NULL)
  {
    fprintf(to, "%s", code->str);
    if (code->type != NEWLINE)
      fputc(' ', to);
    code = code->next;
  }
}

/// @brief exit protocol for macroParser
/// @param curLineHead head of token line in which error was found
/// @param macrosHead head of macro list of macros in file
/// @param lastMacro last macro parsed
/// @param lineNo line number in which error occoured
/// @param errNo error number
/// @param badToken token in which error occoured
/// @param fileName name of file parsed
/// @param from file parsed (".as" file)
/// @param to file parsed to (".am" file)
/// @return
int exitProtocol(tokenNode *curLineHead, macroNode *macrosHead, macroNode *lastMacro, int lineNo, int errNo, tokenNode *badToken, const char *fileName, FILE *from, FILE *to)
{
  // close and delete files
  fclose(from);
  fclose(to);
  removeDotFile(fileName, "am");

  tokenNode tok = cpyToken(badToken);

  freeTokenList(curLineHead);   // free all tokenNodes
  if (macrosHead->next != NULL) // terminate the macro list (macro list always ends in a "empty" malloced macroNode waiting for next value)
  {
    free(lastMacro->next);
    lastMacro->next = NULL;
  }

  freeMcrList(macrosHead); // free all macros

  printMcrErr(errNo, lineNo, tok.str, fileName);
  exit(errNo);
}

/// @brief print an error message
/// @param errNo error number to print
/// @param lineNo line of error
/// @param str token that caused error
/// @param fileName file where error occoured
void printMcrErr(int errNo, int lineNo, char *str, const char *fileName)
{
  fprintf(stderr, "%s.as:%d: Error - ", fileName, lineNo);
  if (errNo == ERR_MISSING_MCR_NAME)
    fprintf(stderr, "Missing identifier\n");
  if (errNo == ERR_UNEXPECTED_TOKEN)
    fprintf(stderr, "Unexpected token (%s)\n", str);
  if (errNo == ERR_INVALID_MCR_NAME)
    fprintf(stderr, "Invalid macro name (%s)\n", str);
  if (errNo == ERR_MCR_REDEF)
    fprintf(stderr, "Redefenition for macro (%s)\n", str);
  if (errNo == ERR_INVALID_MCREND)
    fprintf(stderr, "Unexpected identifier (%s)\n", str);
}