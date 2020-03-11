/*
   File:     symbol.c
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 00/03/31 Reviewed
   Desc:     A simple package for symbol handling.
*/

#include <stdio.h>     /* Standard I/O             */
#include <string.h>    /* Standard string-routines */
#include <stdlib.h>    /* Standard library-routines */
#include "symbol.h"
#include "Obelix.h"

/*int nameErrors = 0;*/

int fFree[] = {0, 0};         /* First free place in each symboltable   */

char* var_func[] = {"mode", "id"};

char symbolTable[2][tableMaxSize][idMaxSize];

void install (int table, char *id, int line)
{
  if (table == LOCAL && lookup (LOCAL, id) == 1) {
    fprintf (stderr, "Error at line %d: variable %s already declared\n", line, id);
    noErrors++;
  }
  else if (table == GLOBAL && lookup (GLOBAL, id) == 1) {
    fprintf (stderr, "Error at line %d: mode %s already declared\n", line, id);
    noErrors++;
  }
  else if (fFree[table] <= tableMaxSize)
    strncpy (symbolTable[table][fFree[table]++], id, idMaxSize - 1);
  else {
	 fprintf (stderr, "\n\rSymboltable is full,");
	 fprintf (stderr, "change the tableMaxSize in symbol.h, exiting...\n\r");
    exit (0);
  }
}

int lookup (int table, char* id)
{
  int x = 0;
  
  if (fFree[table] > 0)
    for (x = 0;x < fFree[table]; x++)
      if  (strncmp(symbolTable[table][x], id, idMaxSize) == 0) return(1);
  return(0);
}

void resetSymbolTable (table)
{
  fFree[table] = 0;
}

void printSymbolTable (table)
{
  int i=0;
  
  while (i < fFree[table])
    fprintf (stderr, "\n\r Var: %s ", symbolTable[table][i++]);
}

void checkId (int table, char* id, int line)
{
  if (lookup (table, id) == 0) {
    fprintf (stderr, "Error at line: %d, accessed %s %s not declared\n",
	     line, var_func[table], id);
    noErrors++;
  }
}








