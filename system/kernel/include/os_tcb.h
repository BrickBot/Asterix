/*
##################################################
##  ASTERIX PROJECT                             ##
##                                              ##
## Subproject   : Asterix, realtime kernel      ##
##                                              ##
## COPYRIGHT (c) 2000                           ##
##################################################
*/
/*
**************************************************
**  File        : tcb.h                         **
**  Date        : 2000-03-07                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--------------------------------------------------
*/
#ifndef __TCB_H__
#define __TCB_H__
#include <hal.h>
#include <os_waitfree.h>

#define IDLETASK        0
#define FIRSTTCB        0
#define LASTTCB         3


/* Task states  */
#define WAITING         0
#define READY           1
#define RUNNING         2
#define SIG_BLOCKED     3

/* TCB */
typedef struct _TCB
{
    period_t            T;                   /* Periodtid */
    offset_t            O;                   /* Offset */
    deadline_t          DL;                  /* Deadline */
    programcounter_t    startAdress;         /* void StartAddress(void *) */
    void*               param;               /* Parameters to task */ 
    errorfunc_t         errorHandler;        /* void ErrorFunction(uint16) */
    ccreg_t             ConditionCodeReg;    /* Default value of ccr */
    bufctrl_t*          bufpointer;          /* pointer to waitfreecontrol */
    semaphorelist_t*	semaphorelist;       /* Semaphores owned by the task */
}TCB_t;

/* Dispatch */
typedef struct _Dispatch
{
    taskstate_t         STATE;                  /* Taskstate */
    lefttodeadline_t    LDL;                    /* Lefttodeadline */
    programcounter_t   	PC;                     /* Programcounter */
    ccreg_t             CCR;                    /* Condition Code Register */
    cpureg_t            GPR0;                   /* Register 0 */
    cpureg_t            GPR1;                   /* Register 1 */
    cpureg_t            GPR2;                   /* Register 2 */
    cpureg_t            GPR3;                   /* Register 3 */
    cpureg_t            GPR4;                   /* Register 4 */
    cpureg_t            GPR5;                   /* Register 5 */
    cpureg_t            GPR6;                   /* Register 6 */
    lefttoperiod_t      LDP;                    /* Lefttoperiod */
    priority_t          current_priority;       /* The current priority of the task */
    priority_t          original_priority;      /* The origin priority of the task */
}dispatch_t;


/*
    The actual lists (TCBList and displist) are located in the file os_tasks.h.
*/
#endif /* __TCB_H__ */
