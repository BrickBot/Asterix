/**************************************************************
    ##################################################
        ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################

  Hardware Adaption Layer (HAL)
  Hitachi h8/300

  Task switch functions that are target cpu specific.
  Contains all functionallity neccessary for doing a
  irq or timer interrupt context switch.


  File:     hal_taskswitch.S
  Date:     2001-08-08
  Authors:  Henrik Thane, Daniel Sundmark,
            Anders Petterson, Andreas Engberg

**************************************************************/

/***************************************************************
  Includes
 **************************************************************/

#include <hal_assembler.h>
#include <os_tasks_inc.h>


/***************************************************************
  Defines
 **************************************************************/


/***************************************************************
  External functions
 **************************************************************/
        .extern     _getNextTask
        .extern     _dispatch
        .extern     _hal_t_return
        .extern     _updateTCBList


/***************************************************************
  Function prototypes
 **************************************************************/
        .global     _hal_timertick

/***************************************************************
   External variables
 **************************************************************/
        .extern     _curr_disp

/**************************************************************
  The TASKSWITCH module
**************************************************************/
        .name       TIMERTICK_TASKSWITCH


; /* Tell the assembler that this belong to the code segment */
        .section    .text

/**************************************************************
  void hal_timertick(void)

  This is the entry to the kernel that is initiated by the
  timer interrupt. We update the tick counter and observe if
  there are any new tasks that need to be put into the ready
  queue. After update we dispatch the highest priority task in
  the ready queue.

**************************************************************/
_hal_timertick:


        ; Acknowledge the timer interrupt
                mov.w   #0xff91,r6
                bclr    #0x03,@r6


        ; /* Save register in displist */
                mov.w   @_curr_disp,r6          ; /* Address to entry in displist for old task in r0 */
                mov.w   r0,@(REG0,r6)           ; /* Save register 1 in displist */
                mov.w   r1,@(REG1,r6)           ; /* save register 0 in displist */
                mov.w   r2,@(REG2,r6)           ; /* save register 2 in displist */
                mov.w   r3,@(REG3,r6)           ; /* save register 3 in displist */
                mov.w   r4,@(REG4,r6)           ; /* save register 4 in displist */
                mov.w   r5,@(REG5,r6)           ; /* save register 5 in displist */
 
                mov.w   @(R6_STACK,sp),r5
                mov.w   r5,@(REG6,r6)           ; /* save register 6 in displist */
                
                mov.w   @(CCR_STACK,sp),r5
                mov.w   r5,@(REGCCR,r6)

                mov.w   @(PC_STACK,sp),r5
                mov.w   r5,@(REGPC,r6)
	
                jsr     @_updateTCBList      ; Update task attribute in TCBList
                jsr     @_getNextTask        ; get the next task with highest priority in readyqueue
                jsr     @_dispatch       ; dispatch highest priority task
                mov.w   #8,r1           ; remove the old content of the stack and
                add.w   r1,sp

        /*-----------------------------------------------------------
        Return value from dispatch (in register r0) have two purposes.
    If bit 0 is set a taskswitch must be performed
    If bit 0 is cleared, no taskswitch is to be done

    If bit 1 is set, the start address of t_return is pushed on stack
    It bit 1 is cleared, no action taken

    We use bit 1 in the return value to control if the start address must
    be pushed on the stack or not. By doing this we eliminate the need
    of a explicit call to t_return() in user tasks.

        -----------------------------------------------------------*/

                btst    #1,r0l
                beq nopush_t_return
                mov.w   #_hal_t_return,r0
                push    r0
nopush_t_return:

                mov.w   @_curr_disp,r0
                mov.w   @(REGPC,r0),r1
                push    r1
                mov.w   @(REGCCR,r0),r1
                push    r1

                ; /* restore all registers to what is recorded in the dispatch_struct.*/
                mov.w   @(REG6,r0),r6
                mov.w   @(REG5,r0),r5
                mov.w   @(REG4,r0),r4
                mov.w   @(REG3,r0),r3
                mov.w   @(REG2,r0),r2
                mov.w   @(REG1,r0),r1
                mov.w   @(REG0,r0),r0       ;  /* Registers are set to the new tasks  */

                rte

/**************************************************************
  End of TIMERTICK_TASKSWITCH module
**************************************************************/
                .END
