#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macroParser.h"
#include "macro.h"
#include "../lexer/lexer.h"

/// @brief breate a new macroNode (using malloc for macro fields - call free)
/// @param name name of macro
/// @return the macroNode created
macroNode newMcrNode(char *name)
{
  macroNode m;
  m.name = (char *)malloc(strlen(name));
  strcpy(m.name, name);
  m.next = NULL;
  m.code = (tokenNode *)malloc(sizeof(tokenNode));
  return m;
}

/// @brief init a new empty macroList (uses malloc - call free)
/// @param
/// @return pointer to macro list
macroNode *newMcrList(void)
{
  macroNode *mcrList = (macroNode *)malloc(sizeof(macroNode));
  mcrList->next = NULL;
  mcrList->name = NULL;
  mcrList->code = NULL;
  return mcrList;
}

/// @brief create a new empty macro (uses malloc - call free)
/// @param
/// @return pointer to macro initialized
macroNode *emptyMcr(void)
{
  macroNode *m = (macroNode *)(malloc(sizeof(macroNode)));
  m->name = NULL;
  m->code = NULL;
  m->next = NULL;
  return m;
}

/// @brief check if a macro exist in a macro list
/// @param macros macro lisr to check in
/// @param str name of macro to find
/// @return pointer to macro, or NULL if macro wasnt found
macroNode *mcrDefined(macroNode *macros, char *str)
{
  macroNode *cur = macros;
  while (cur->name != NULL)
  {
    if (strcmp(cur->name, str) == 0)
      return cur;
    cur = cur->next;
  }
  return NULL;
}

/// @brief check if a macrend decleration is valid
/// @param head head of current token line in which macrend dec was found
/// @return non-zero value if macrend decleration is valid else 0
int validMcrEnd(tokenNode *head)
{
  return !(head->type != END_MACRO || (head->next != NULL && head->next->type != NEWLINE));
}

/// @brief check if a macr decleration is valid
/// @param head head of current token line in which macr dec was found
/// @param macros head of macro list of macros defined in file
/// @return non-zero value if valid else 0
int validMcrDec(tokenNode *head, macroNode *macros)
{
  tokenNode *name = head->next;
  if (name->type == NEWLINE) // macro name was not provided
    return ERR_MISSING_MCR_NAME;

  // tokens before/after macr decleration
  if (head->type != MACRO || name->next->type != NEWLINE)
    return ERR_UNEXPECTED_TOKEN;

  if (name->type != SYMBOL) // invalid macro name
    return ERR_INVALID_MCR_NAME;

  if (mcrDefined(macros, name->str)) // macro already defined
    return ERR_MCR_REDEF;

  return 0;
}

/// @brief free a macro list
/// @param mcr macro list to free
void freeMcrList(macroNode *mcr)
{
  macroNode *cur = mcr;
  macroNode *next = cur;
  while (cur != NULL)
  {
    next = cur->next;
    free(cur->name);
    freeTokenList(cur->code);
    free(cur);
    cur = next;
  }
}

/// @brief end macro protocol
/// @param flag pointer to macroflag
/// @param codePrev pointer to last code token inserted to code
/// @param curMcr current macro to end
void endMacro(int *flag, tokenNode *codePrev, macroNode *curMcr)
{
  *flag = 0;
  free(codePrev->next); // while parsing the macro the code always has an "empty" malloced token in end that is expecting the next token
  codePrev->next = NULL;
  curMcr->next = emptyMcr();
}

/// @brief add a code token to a macro code
/// @param addTo code to add to
/// @param token token to add
void addCode(tokenNode *addTo, tokenNode *token)
{
  *addTo = cpyToken(token);
  addTo->next = (tokenNode *)malloc(sizeof(tokenNode));
}