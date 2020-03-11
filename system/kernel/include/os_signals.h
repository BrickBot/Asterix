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
**  File        : signal.h                      **
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
#ifdef __SIGNALS__

#ifndef __SIGNALS_H__
#define __SIGNALS_H__

#include <hal_kertypes.h>

typedef struct _Signal
{
    taskid_t    *signalowners;              /* Tasks that may wait for this signal */
    uint16      nrtasks;                    /* Number of tasks that the signal can handle */
    taskid_t    *blklist;                   /* Tasks that are currently waiting */
    uint16      nrblocked;
}signal_t;


extern void raiseSignal(sigid_t sigid);

#endif /* __SIGNALS_H__ */

#endif

