/*
   File:     symbol.h
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 00/03/30 Reviewed
   Desc:     A simple package for symbol handling.
*/

#define  tableMaxSize 1030          /* The size of the symboltable       */
#define  idMaxSize 256             /* The size of each entry (id)       */

#define  GLOBAL 0 /* Global stores mode ids                       */
#define  LOCAL  1 /* Local stores task- and rescources ids in a mode */

/*extern int nameErrors;*/

extern char symbolTable[2][tableMaxSize][idMaxSize];

extern void install(int table, char* id, int line);
extern int lookup (int table, char* id);
extern void resetSymbolTable (int table);
extern void printSymbolTable (int table);

extern void checkId (int table, char* id, int line); /* Id declared ?  */




