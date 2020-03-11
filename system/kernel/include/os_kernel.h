/**************************************************************
    ##################################################
        ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################

    Kernel.h

    Basic kernel definitions and includes

    Date:     2001-03-24
    Authors:  Henrik Thane, Daniel Sundmark,
              Anders Petterson, Andreas Engberg

**************************************************************/
#ifndef __KERNEL_H__
#define __KERNEL_H__

/***************************************************************
  Includes
***************************************************************/
#include <hal.h>
#include <os_tcb.h>
#include <os_semaphore.h>
#include <os_signals.h>
#include <os_tasks.h>
#include <os_waitfree.h>
#include <os_errno.h>
#include <os_axlib.h>

/***************************************************************
  Defines
***************************************************************/

#define MISSEDDEADLINE  0

/***************************************************************
  Function prototypes
***************************************************************/

extern void     getNextTask(void);
extern int      dispatch(void);
extern void     initTask( taskid_t taskid );
extern tcbid_t  Task2TCB(taskid_t task);
extern void     updateTCBList( void );
extern void     moveToWaiting( taskid_t taskid );
extern void     moveToReady( taskid_t taskid );
extern ticks_t  getTicks(void);
extern void     hal_clearRegisters(void);
extern void     userinit(void);

extern void     os_changeMode( uint16 mode );

/***************************************************************
  Global variables
***************************************************************/
extern TCB_t*       curr_tcb;
extern dispatch_t*  curr_disp;
extern taskid_t     nexttask; /* Checkup initial value to set here */
extern taskindex_t  ExecTask; /* The running task */
extern ticks_t      ticks; /*  System clock */

#endif  /* __KERNEL_H__ */
