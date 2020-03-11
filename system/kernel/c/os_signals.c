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
**  File        : signals.c                   **
**  Date        : 2000-02-27                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--       Functions for handling of signals.     --
--       Signals can be used to handle external --
--       and internal interrupts.               --
--                                              --
--------------------------------------------------
*/

#include <os_kernel.h>
#ifdef  __SIGNALS__


/*
**************************************************
**  Name        :                               **
**      void raiseSignal(sigid_t sigid)         **
**  Usage       :                               **
**      raiseSignal( signalnumber );            **
**                                              **
**  Arguments   : The number of the signal      **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : Wakes up all tasks waiting    **
**                waiting for signal number     **
**                                              **
**  Use         :                               **
**                yield();                      **
**                initTask();                   **
**                moveToReady();                **
**                                              **
**************************************************
*/
void raiseSignal(sigid_t sigid)
{
#ifdef __TIME_MEASURE__
    return;
#else
    signal_t    *sig=&signals[sigid];
    sigid_t     max=signals[sigid].nrtasks;
    sigid_t     i=0;
    taskid_t    currtask=ExecTask;
    tcbid_t     blkpos=0;
    taskid_t    blk=0;

   hal_disable_interrupt();
   if(sigid > no_signals)
        TCBList[ExecTask].errorHandler(ERROR_ILLEGAL_SIGNAL);

    for(;i<max;i++)
    {
        if(currtask == sig->signalowners[i])
        {
            for(i=0;i<sig->nrblocked;i++){
                blk=sig->blklist[i];
                blkpos=blk;

//                initTask(blkpos);
                moveToReady(blk);

#ifdef __WAITFREE__
              updateWaitfree(blkpos);
#endif

            }
            hal_yield();
            return;
        }
    }

    TCBList[currtask].errorHandler(ERROR_ILLEGAL_SIGNAL_ACCESS);
#endif
}

#endif  /* __SIGNALS__ */
