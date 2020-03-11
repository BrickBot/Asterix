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
**  File        : timer.h                       **
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
#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__
#include <hal_types.h>

extern void     hal_initTimer(void (*timerfunc)(void));
extern void     hal_startTimer(void);
extern void     hal_stopTimer(void);
extern uint16   hal_readTimer(void);
extern ticks_t  hal_getFRCval(void );

extern uint16   timerVal;
#endif  /* __HAL_TIMER_H__ */
