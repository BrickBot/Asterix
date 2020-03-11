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
**  File        : hal_kertypes.h                **
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
#ifndef __HAL_KERTYPES_H__
#define __HAL_KERTYPES_H__
#define SZ_EVENT 12

#include <hal_types.h>

/* Types used by the kernel ( ?and for the moment also by the user? ) */

typedef uint16      bitpos_t;
typedef uint16      bittype_t;
typedef uint16      bytepos_t;
typedef uint16      ccreg_t;
typedef uint16      deadline_t;
typedef uint16      offset_t;
typedef uint16      period_t;
typedef uint16      priority_t;             /* User */
typedef void_func_p programcounter_t;
typedef uint16      cpureg_t;
typedef uint16      semaphorelist_t;
typedef uint16      semid_t;                /* User */
typedef uint16      semowner_t;
typedef uint16      semtask_t;
typedef uint16      sigid_t;                /* User */
typedef uint16      sigtask_t;
typedef int16       stack_t;                /* user ?? */
typedef uint16      subticks_t;
typedef uint16      taskid_t;               /* User */
typedef uint16      taskindex_t;
typedef uint16      taskstate_t;
typedef uint16      tcbid_t;
typedef uint16      ticks_t;                /* user ?? */

typedef void (*errorfunc_t)( uint16 );

typedef stack_t*    stackpointer_t;
typedef deadline_t  lefttodeadline_t;
typedef period_t    lefttoperiod_t;

#endif  /* __HAL_KERTYPES_H__ */









