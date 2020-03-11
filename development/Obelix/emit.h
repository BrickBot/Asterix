/*
   File:     emit.h
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 1.0, 00/04/18 Obelix back-end.
   Desc:     Routines that produces c-code to be compiled into a
             Asterix system.
*/

#define SZ_CONTROLFLOWEVENT 12
#define CFILENAME "/system/kernel/c/os_tasks.c"
#define HFILENAME "/system/kernel/include/os_tasks.h"
#define INCFILENAME "/system/kernel/include/os_tasks_inc.h"
#define FPSCALCFILENAME "/system/asterix.fps"
#define BOUNDTFILENAME "/system/boundt.bat"
#define JAVAFILENAME "/development/debug/java/Tasks.java"

typedef tTree *tTreePtr;

void createFile(tTree root);
