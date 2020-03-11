/*
   File:     Obelix04.y
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 0.4, 00/01/25 waitfree rule extended.
	          00/03/30
   Desc:     The syntax analysator for Obelix Configuration
             Language
*/
%{
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <Obelix.h>
#include <syntree.h>
#include <symbol.h>
#include <semact.h>
#include <emit.h>

  tTree root = NO_NODE; /* root of the syntax-tree */
  int noErrors=0;
  int noWarnings=0;
%}

%union { 	        /* type of elements on the parse-value stack */
  tTree yyNode;
  intStruct yyInt;
  stringStruct yyString;
  sysModeStruct yySysMod;
  typeStruct yyType;
  int yyLineNr;
};


/* Rule types on the parse stack */
%type  <yyNode> file args error_routine
%type  <yyNode> modes mode tasks task
%type  <yyNode> hardtasks softtasks activator resources communication
%type  <yyNode> waitfrees waitfree readers synchronization signals
%type  <yyNode> signal users semaphores semaphore reader user
%type  <yyLineNr> systemmode ram resolution num_buf

/* Token types on the parse stack */
%token <yyInt>    INT_CONST
%token <yyString> STRING_CONST ID
%token <yySysMod> SYSMODE
%token <yyLineNr> NORMAL TEST RAM MODE HARD_TASK SOFT_TASK ACTIVATOR
%token <yyLineNr> OFFSET DEADLINE ROUTINE ARGS_T ERR_ROUTINE WAITFREE
%token <yyLineNr> WRITER READER SIGNAL USER SEMAPHORE SYSTEMMODE RESOLUTION_T
%token <yyLineNr> PRIORITY NUM_BUF TYPE STACK
%token <yyLineNr> PERIODIC APERIODIC MAX_INTERARRIVAL_TIME
%token <yyLineNr> PERIOD_TIME SPORADIC MIN_INTERARRIVAL_TIME

%start file

%%
/* Grammar specification below */

file					: systemmode ram modes {root = mProgram($1, $2, $3, 0);}
						;

systemmode				: SYSTEMMODE '=' SYSMODE ';' {$$ = $3.modeType;}
						;

ram						: RAM '=' INT_CONST ';' {$$ = $3.intVal;}
						;

modes					: modes mode {if($1) $$ = connectNode($1, $2, kMode); else $$ = $2;}
						| mode {$$ = $1;}
						;

mode					: MODE ID {install(GLOBAL, $2.strVal, $2.lineNr);}
						  '{' resolution tasks resources '}'
						  ';' {$$ = mMode($2.strVal, $5, $6, $7, $1);
						       resetSymbolTable(LOCAL);}
						;

resolution				: RESOLUTION_T '=' INT_CONST ';' {$$ = $3.intVal;}
						;

tasks					: hardtasks softtasks {$$ = mTasks($1, $2, 0);}
						;

hardtasks				: hardtasks HARD_TASK task {if($1) $$ = connectNode($1, $3, kTask);
							                      else $$ = $3; }
						| /* epsilon */ {$$ = NO_NODE;}
						;

softtasks				: softtasks SOFT_TASK task {if($1) $$ = connectNode($1, $3, kTask);
					    		                  else $$ = $3; }
						| {$$ = NO_NODE;}
						;

task					: PERIODIC ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{'
								PERIOD_TIME '=' INT_CONST ';'
								OFFSET '=' INT_CONST ';'
								DEADLINE '=' INT_CONST ';'
								PRIORITY '=' INT_CONST ';'
								STACK '=' INT_CONST ';'
								ROUTINE '=' ID ';'
								args
								error_routine
						  '}' ';' {$$ = mTask(kPeriodic,
											  $2.strVal,
											  $7.intVal,
											  NOT_DEFINED /* NOT A SPORADIC TASK */,
											  NOT_DEFINED /* NOT A SPORADIC TASK */,
											  NO_NODE /* NO SIGNAL ACTIVATOR */,
											  $11.intVal,
											  $15.intVal,
											  $19.intVal,
											  $23.intVal,
											  $27.strVal,
											  $29,
											  $30,
											  $2.lineNr);}
						| APERIODIC ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{'
								ACTIVATOR '=' activator ';'
								OFFSET '=' INT_CONST ';'
				                DEADLINE '=' INT_CONST ';'
						        PRIORITY '=' INT_CONST ';'
								STACK '=' INT_CONST ';'
			                 ROUTINE '=' ID ';'
				             args
					         error_routine
						  '}' ';' {$$ = mTask(kAperiodic,
											  $2.strVal,
											  NOT_DEFINED /* NOT A PERIODIC TASK */,
											  NOT_DEFINED /* NOT A SPORADIC TASK */,
											  NOT_DEFINED /* NOT A SPORADIC TASK */,
											  $7,
											  $11.intVal,
											  $15.intVal,
											  $19.intVal,
											  $23.intVal,
											  $27.strVal,
											  $29,
											  $30,
											  $2.lineNr);}
						| SPORADIC ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{'
								MIN_INTERARRIVAL_TIME '=' INT_CONST ';'
								MAX_INTERARRIVAL_TIME '=' INT_CONST ';'
								ACTIVATOR '=' activator ';'
								OFFSET '=' INT_CONST ';'
						        DEADLINE '=' INT_CONST ';'
								PRIORITY '=' INT_CONST ';'
								STACK '=' INT_CONST ';'
								ROUTINE '=' ID ';'
								args
								error_routine
						  '}' ';' {$$ = mTask(kSporadic,
											  $2.strVal,
											  NOT_DEFINED /* NOT A PERIODIC TASK */,
											  $7.intVal,
											  $11.intVal,
											  $15,
											  $19.intVal,
											  $23.intVal,
											  $27.intVal,
											  $31.intVal,
											  $35.strVal,
											  $37,
											  $38,
											  $2.lineNr);}
						;

activator				: INT_CONST {$$ = mIntConst($1.intVal, $1.lineNr);}
						| ID {$$ = mStringConst($1.strVal, $1.lineNr);}
						;

args		            : ARGS_T '=' STRING_CONST ';' {$$ = mStringConst($3.strVal, $1);}
						| {$$ = NO_NODE;}
						;

error_routine		    : ERR_ROUTINE '=' ID ';' {$$ = mStringConst($3.strVal, $1);}
						| {$$ = NO_NODE;}
						;

resources		        : communication synchronization {$$ = mResources($1, $2, 0);}
						;

communication			: waitfrees {$$ = mCommunications($1, 0);}
						;

waitfrees				: waitfrees waitfree {if($1) $$ = connectNode($1, $2, kWaitFree); else $$ = $2;}
						| {$$ = NO_NODE;}
						;

waitfree				: WAITFREE
						  ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{'
						  WRITER
						  '='
						  ID {checkId(LOCAL, $7.strVal, $7.lineNr);}
						  ';'
						  readers
						  num_buf
						  TYPE '=' STRING_CONST ';'
						  '}'
						  ';' {$$ = mWaitFree($2.strVal, $7.strVal, $10, $11, $14.strVal, $1);}
						;

num_buf					: NUM_BUF '=' INT_CONST ';' {$$ = $3.intVal;}
						|{$$ = NOT_DEFINED;}
						;

readers					: readers reader {$$ = connectNode($1, $2, kUser);}
						| reader {$$ = $1;}

reader					: READER '=' ID ';' {checkId(LOCAL, $3.strVal, $3.lineNr);
								             $$ = mUser($3.strVal, $3.lineNr);}
		                ;

synchronization			: signals semaphores {$$ = mSynchronizations($1, $2, 0);}
						;

signals					: signals signal {if($1) $$ = connectNode($1, $2, kSignal); else $$ = $2;}
						| {$$ = NO_NODE;}
						;

signal					: SIGNAL ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{' users '}' ';' {$$ = mSignal($2.strVal, $5, $1);}
						;

users					: users user {$$ = connectNode($1, $2, kUser);}
						| user {$$ = $1;}
						;

user					: USER '=' ID ';' {checkId(LOCAL, $3.strVal, $3.lineNr);
										   $$ = mUser($3.strVal, $3.lineNr);}
						;

semaphores				: semaphores semaphore {if($1) $$ = connectNode($1, $2, kSemaphore); else $$ = $2;}
						| {$$ = NO_NODE;}
						;

semaphore				: SEMAPHORE ID {install(LOCAL, $2.strVal, $2.lineNr);}
						  '{' users '}' ';' {$$ = mSemaphore($2.strVal, $5, $1);}
						;

%%



int main(int argc, char *argv[]){
  int syntaxErrors = 0;
  char *p;
  extern FILE *yyin;

  if (argc <= 1) {
    fprintf (stderr, "\n\rYou must specify an input file to parse...\n\r");
    return (1);
  }

  /* make sure that the user specifies a .obx file as input */
  p = strstr (argv[1], ".obx");

  if (p == NULL) {
    fprintf (stderr, "\n\rInput file has to have the .obx extension...\n\r");
    return (1);
  }

   if (!(yyin = fopen (argv[1], "r+"))) {
    fprintf (stderr, "\n\rCould not open input file %s\n\r", argv[1]);
    return (1);
   }

   resetSymbolTable(GLOBAL);
   resetSymbolTable(LOCAL);

     fprintf(stderr, "\n\r%d typeErrors\n", noErrors);

   /* parse the input file and build up the syntax tree, our internal
      representation */
   syntaxErrors = yyparse();

     fprintf(stderr, "\n\r%d typeErrors\n", noErrors);

   /* if any syntax errors are obtained, exit */
   if(syntaxErrors){
     fprintf(stderr, "\n\rFel fel fel....%d syntaxErrors\n",
	     syntaxErrors);
     exit(1);
   }

   /* do the sematic controls */
   check_signal_arg(root);
   check_task_arg(root);
   check_wait_free(root);
   make_unique_prio(root);
   check_semaphores(root);
   checkStacks(root);

   /* printTaskPrio(root);*/
   /* printSemaphoreCeil(root);*/

   /* if the semantic controls caused any errors, exit */
   if(noErrors){
     fprintf(stderr, "\n\r%d typeErrors\n", noErrors);
      exit(1);
    }

   fprintf(stdout, "\nPassed all controls with %d warnings and %d error\n",
	   noWarnings, noErrors);

   /* printTree(root); */


   /* Backend part */

   /* Create .h file */
   createFile(root);

   freeTree(root);

   return 0;
}

int yyerror(char *s)
{
  extern int yylineno;
  fprintf (stderr, "\n\rA %s at line %d\n",s, yylineno);

  return 1;
}
