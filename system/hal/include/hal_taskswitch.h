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
**  File        : taskswitch.h                  **
**  Date        : 2000-03-07                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
#ifndef __TASKSWITCH_H__
#define __TASKSWITCH_H__

extern void hal_timertick(void);
extern void hal_t_return(void);
extern void hal_yield(void);
extern void hal_irqyield(void);

#endif  /* __TASKSWITCH_H__ */
