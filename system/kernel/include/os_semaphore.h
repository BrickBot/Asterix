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
**  File        : semaphor.h                    **
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
#ifdef  __SEMAPHORE__

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <hal_kertypes.h>

/* Semaphore */
typedef struct _Semaphore
{
    taskid_t*   semaphoreowners;			/* (semowner_t)Task that are allowed to own a semaphore */
    uint16      nrtasks;
    priority_t  priority_ceiling;			/* (sempos_t)The priority ceiling of the semaphore */
}semaphore_t;
            
extern void releaseSemaphore( semid_t semid );
extern void getSemaphore(semid_t semid);

#endif	/* __SEMAPHORE_H__ */

#endif
