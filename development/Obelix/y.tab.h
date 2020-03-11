/* A Bison parser, made by GNU Bison 1.875b.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_CONST = 258,
     STRING_CONST = 259,
     ID = 260,
     SYSMODE = 261,
     NORMAL = 262,
     TEST = 263,
     RAM = 264,
     MODE = 265,
     HARD_TASK = 266,
     SOFT_TASK = 267,
     ACTIVATOR = 268,
     OFFSET = 269,
     DEADLINE = 270,
     ROUTINE = 271,
     ARGS_T = 272,
     ERR_ROUTINE = 273,
     WAITFREE = 274,
     WRITER = 275,
     READER = 276,
     SIGNAL = 277,
     USER = 278,
     SEMAPHORE = 279,
     SYSTEMMODE = 280,
     RESOLUTION_T = 281,
     PRIORITY = 282,
     NUM_BUF = 283,
     TYPE = 284,
     STACK = 285,
     PERIODIC = 286,
     APERIODIC = 287,
     MAX_INTERARRIVAL_TIME = 288,
     PERIOD_TIME = 289,
     SPORADIC = 290,
     MIN_INTERARRIVAL_TIME = 291
   };
#endif
#define INT_CONST 258
#define STRING_CONST 259
#define ID 260
#define SYSMODE 261
#define NORMAL 262
#define TEST 263
#define RAM 264
#define MODE 265
#define HARD_TASK 266
#define SOFT_TASK 267
#define ACTIVATOR 268
#define OFFSET 269
#define DEADLINE 270
#define ROUTINE 271
#define ARGS_T 272
#define ERR_ROUTINE 273
#define WAITFREE 274
#define WRITER 275
#define READER 276
#define SIGNAL 277
#define USER 278
#define SEMAPHORE 279
#define SYSTEMMODE 280
#define RESOLUTION_T 281
#define PRIORITY 282
#define NUM_BUF 283
#define TYPE 284
#define STACK 285
#define PERIODIC 286
#define APERIODIC 287
#define MAX_INTERARRIVAL_TIME 288
#define PERIOD_TIME 289
#define SPORADIC 290
#define MIN_INTERARRIVAL_TIME 291




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 24 "Obelix.y"
typedef union YYSTYPE { 	        /* type of elements on the parse-value stack */
  tTree yyNode;
  intStruct yyInt;
  stringStruct yyString;
  sysModeStruct yySysMod;
  typeStruct yyType;
  int yyLineNr;
} YYSTYPE;
/* Line 1252 of yacc.c.  */
#line 118 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



