/*
   File:     Obelix.h
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 00/03/30 
   Desc:     Parsing constants and types for the parse stack
*/

#define NORMAL_CONST	101
#define TEST_CONST		102
#define INT8			200;
#define INT16			201;
#define STRING_SIZE		256
#define NOT_DEFINED		-1

typedef struct{
  int lineNr;
  int intVal;
}intStruct;

typedef struct{
  int lineNr;
  char strVal[STRING_SIZE];
}stringStruct;

typedef struct{
  int lineNr;
  int modeType;
}sysModeStruct;

typedef struct{
  int lineNr;
  int type;
}typeStruct;

/*typedef char string[STRING_SIZE];*/
extern int noErrors;
extern int noWarnings;
