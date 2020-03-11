/************************************************************
   File:     Obelix.lex
   Autor:    Anders Davidsson
   Revision: 0.4, 00/01/25 Comments can not include newline.
                  00/03/30
   Desc:     The lexical analysator for Obelix configuration
             language
************************************************************/

%{
#include <stdio.h>
#include <string.h>
#include <Obelix.h>
#include <syntree.h>
#include <y.tab.h>
int yylineno = 1;
%}

COMMENT "/*"([^*/]|[^*]"/"|"*"[^/])*"*/"
NEWCOMMENT "/@"([^*/]|[^*]"/"|"*"[^/])*"@/"
STR     "\""([^\"])*"\""
delim   [ \t]
WS      {delim}+
DIGIT   [0-9]
USCORE  [_]
LETTER  [A-Za-z]
ID      {LETTER}({LETTER}|{DIGIT}|{USCORE})*
%%

{WS}			;
{COMMENT}		;
{NEWCOMMENT}	;

SYSTEMMODE				{yylval.yyLineNr = yylineno; return(SYSTEMMODE);}

NORMAL					{yylval.yySysMod.lineNr = yylineno;
						 yylval.yySysMod.modeType = NORMAL_CONST;
						 return (SYSMODE);}

TEST					{yylval.yySysMod.lineNr = yylineno;
						 yylval.yySysMod.modeType = TEST_CONST;
						 return (SYSMODE);}

RAM						{yylval.yyLineNr = yylineno; return (RAM);}

MODE					{yylval.yyLineNr = yylineno; return (MODE);}

RESOLUTION				{yylval.yyLineNr = yylineno; return (RESOLUTION_T);}

HARD_TASK				{yylval.yyLineNr = yylineno; return (HARD_TASK);}

SOFT_TASK				{yylval.yyLineNr = yylineno; return (SOFT_TASK);}

ACTIVATOR				{yylval.yyLineNr = yylineno; return (ACTIVATOR);}

OFFSET					{yylval.yyLineNr = yylineno; return (OFFSET);}

DEADLINE				{yylval.yyLineNr = yylineno; return (DEADLINE);}

PRIORITY				{yylval.yyLineNr = yylineno; return (PRIORITY);}

STACK					{yylval.yyLineNr = yylineno; return (STACK);}

ROUTINE					{yylval.yyLineNr = yylineno; return (ROUTINE);}

ARGUMENTS				{yylval.yyLineNr = yylineno; return (ARGS_T);}

ERR_ROUTINE				{yylval.yyLineNr = yylineno; return (ERR_ROUTINE);}

WAITFREE				{yylval.yyLineNr = yylineno; return (WAITFREE);}

WRITER					{yylval.yyLineNr = yylineno; return (WRITER);}

READER					{yylval.yyLineNr = yylineno; return (READER);}

SIGNAL					{yylval.yyLineNr = yylineno; return (SIGNAL);}

USER					{yylval.yyLineNr = yylineno; return (USER);}

SEMAPHORE				{yylval.yyLineNr = yylineno; return (SEMAPHORE);}

TYPE					{yylval.yyLineNr = yylineno; return (TYPE);}

NUM_BUF					{yylval.yyLineNr = yylineno; return (NUM_BUF);}

PERIODIC				{yylval.yyLineNr = yylineno; return (PERIODIC);}

APERIODIC				{yylval.yyLineNr = yylineno; return (APERIODIC);}

SPORADIC				{yylval.yyLineNr = yylineno; return (SPORADIC);}

PERIOD_TIME				{yylval.yyLineNr = yylineno; return (PERIOD_TIME);}


\n     {yylineno++;}

{STR}       {yytext[yyleng-1] = 0;
             strncpy(yylval.yyString.strVal, &yytext[1], STRING_SIZE);
	     yylval.yyString.lineNr = yylineno;
	     return (STRING_CONST);}

{DIGIT}+    {yylval.yyInt.intVal = atoi(yytext);
             yylval.yyInt.lineNr = yylineno;
	     return (INT_CONST);}

{ID}        {strncpy(yylval.yyString.strVal, yytext, STRING_SIZE);
             yylval.yyString.lineNr = yylineno;
	     return (ID);}

.           {return (yytext[0]);} /* any kind of charachter */

%%

int yywrap(){return (1);}


