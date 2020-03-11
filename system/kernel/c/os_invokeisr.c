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
**  File        : invokeisr.c                   **
**  Date        : 2000-02-27                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--    This is a customized verison of the       --
--    raisesignal() function.                   --
--                                              --
--------------------------------------------------
*/
#include <os_kernel.h>
#include <sys_spec.h>

#ifdef __INTERRUPTS__


/*
**************************************************
**  Name        : void invokeISR(sigid_t sigid) **
**  Usage       : invokeISR( signalnumber );    **
**                                              **
**  Arguments   : The number of the signal      **
**                that is assigned to the ISR   **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : To wake up the task that is   **
**                hooked to the IRQ.            **
**                                              **
**  Use         :                               **
**                initTask( tcbid );            **
**                moveToReady( taskid );        **
**                irqyield();                   **
**                                              **
**************************************************
*/
C_task void invokeISR(sigid_t sigid)
{
    taskid_t taskid     = signals[sigid].blklist[0];
    taskid_t exectask   = ExecTask;

    if( sigid > no_signals )
        TCBList[ exectask ].errorHandler(ERROR_ILLEGAL_SIGNAL);

    moveToReady( taskid );
    hal_yield();
}

#endif  /* __INTERRUPTS__ */
