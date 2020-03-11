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
**  File        : errno.h                       **
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
#ifndef __ERRNO_H__
#define __ERRNO_H__

/* ERROR CODES */
#define ERROR                           1000    /* General error */
#define ERROR_MISSEDDEADLINE            1001    /* Code for missed deadline */
#define ERROR_ILLEGAL_SEMAPHORE         1002    /* The semaphore doesn't exist */
#define ERROR_ILLEGAL_SEMAPHORE_ACCESS  1003    /* The task don't have access to this semaphore */
#define ERROR_ILLEGAL_SIGNAL            1004    /* Signal doesn't exist */
#define ERROR_ILLEGAL_SIGNAL_ACCESS     1005    /* Task don't have access to this signal */


#endif  /* __ERRNO_H__ */
