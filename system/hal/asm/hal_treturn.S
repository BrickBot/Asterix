/**************************************************************
    ##################################################
        ASTERIX RTOS                                
        COPYRIGHT (c) 2000, 2001                    
    ##################################################

  Hardware Adaption Layer (HAL)
  Hitatchi H8/300

  Task switch functions that are target cpu specific. 
  Contains all functionallity neccessary for doing a 
  task return context switch.
  

  File:     hal_treturn.s20
  Date:     2001-03-09
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
  Externals
**************************************************************/
        .EXTERN     _initTask
        .EXTERN     _getNextTask
        .EXTERN     _dispatch

      
/***************************************************************
  Function prototypes
**************************************************************/
        .GLOBAL     _hal_t_return
 
/***************************************************************
  External variables
**************************************************************/
        .EXTERN     _ExecTask
        .EXTERN     _curr_disp


/**************************************************************
  The TRETURN_TASKSWITCH module
**************************************************************/
        .NAME       TRETURN_TASKSWITCH
                            
; /* Tell the assembler that this belong to the code segment */
        .section    .text


/**************************************************************
  void hal_t_return(void)

  This entry to the kernel is called to when a task terminates. The task
  is moved to the waiting queue, wating for the arrival of the next 
  periodic activation. The task with the highest priority in the ready  
  queue is dispatched (moved to executing).             

  User task do not have to call t_return explicity, the kernel pushes   
  the adress to t_return the first time a task is invoked in a new period.
    
**************************************************************/
_hal_t_return:
              orc     #0x80,ccr     //disable interrupts


        
              mov.w   @_ExecTask,r0
              jsr     @_initTask

#ifdef __WAITFREE__
              mov.w   @_ExecTask,r0      ; Get the running tasks ID 
              jsr     @_t_returnWaitfree
#endif

              jsr     @_getNextTask        ; Get next task that is ready to execute
              jsr     @_dispatch           ; dispatch


        /*-----------------------------------------------------------
          The return value from dispatch (in register r6) has two 
          purposes:
          *If bit 0 is set a taskswitch must be performed
          *If bit 0 is cleared, no taskswitch is to be done
        
          *If bit 1 is set, the start address of t_return is 
            pushed on stack, this to facilitate correct return to
            kernel when task terminates
          *It bit 1 is cleared, no action is taken
        
          We use bit 1 in the return value to control if the start 
          address must be pushed on the stack or not. By doing this 
          we eliminate the need of an explicit call to 
          t_return() in user tasks.

        -----------------------------------------------------------*/


              btst    #1,r0l
              beq     nopush_t_return
              mov.w   #_hal_t_return,r0
              push    r0
nopush_t_return:

              mov.w   @_curr_disp,r0
              mov.w   @(REGPC,r0),r1          ;Set up the new return address to new running task, hint: we make an rts
              push    r1

      ;Restore all registers from what is recorded in dispatch_struct
              mov.w   @(REG6,r0),r6
              mov.w   @(REG5,r0),r5
              mov.w   @(REG4,r0),r4
              mov.w   @(REG3,r0),r3
              mov.w   @(REG2,r0),r2
              mov.w   @(REG1,r0),r1
              mov.w   @(REG0,r0),r0     
      
              andc    #0x7f,ccr     // enable interrupts
              rts                   // call next task, then address is on the stack
             
/**************************************************************
  End of TRETURN_TASKSWITCH module
**************************************************************/
              .END
