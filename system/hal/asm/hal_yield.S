/**************************************************************
    ##################################################
        ASTERIX RTOS                                
        COPYRIGHT (c) 2000, 2001                    
    ##################################################

  Hardware Adaption Layer (HAL)
  Hitatchi H8/300

  Task switch functions that are target cpu specific. 
  Contains all functionallity neccessary for doing a 
  yield context switch.
  

  File:     hal_yield.s20
  Date:     2001-03-09
  Authors:  Henrik Thane, Daniel Sundmark, 
            Anders Petterson, Andreas Engberg 

**************************************************************/

/***************************************************************
  Includes 
**************************************************************/
#include <hal_assembler.h>
#include <os_tasks_inc.h>

#ifdef MONITORING
  #include <hal_replay_assembler.h>
#endif /* MONITORING */

/***************************************************************
  Defines 
**************************************************************/

         
/***************************************************************
  Externals
**************************************************************/
        .EXTERN     _getNextTask
        .EXTERN     _dispatch
        .EXTERN     _hal_t_return
 
/***************************************************************
  Function prototypes
**************************************************************/
        .GLOBAL     _hal_yield
              
/***************************************************************
  External variables
**************************************************************/
        .EXTERN     _curr_disp


/**************************************************************
  The YIELD module
**************************************************************/
        .NAME       YIELD_TASKSWITCH
                            

                            
; /* Tell the assembler that this belong to the code segment */
        .section    .text


/***********************************************************************
    void hal_yield(void)

    This entry to the kernel is initiated by an access to a semaphore,
    or a signal.
***********************************************************************/
_hal_yield:
          orc     #0x80,ccr     // disable interrupts


              mov.w   r0,@_register0_temp
              mov.w   r6,@_register6_temp
              mov.w   @sp,r0
              mov.w   r0,@_programcounter_temp
            
              ; Save register in displist
              mov.w   @_curr_disp,r0 ; Lägg aktuell address till displist  i r0
              mov.w   r1,@(REG1,r0) ; spara register 1

              mov.w   @_register0_temp,r1
              mov.w   r1,@(REG0,r0)       ; spara register 0
              mov.w   r2,@(REG2,r0)       ; spara register 2
              mov.w   r3,@(REG3,r0)       ; spara register 3
              mov.w   r4,@(REG4,r0)       ; spara register 4
              mov.w   r5,@(REG5,r0)       ; spara register 5
              mov.w   @_register6_temp,r6
              mov.w   r6,@(REG6,r0)   ; spara register 6

              mov.w   @_programcounter_temp,r1
              mov.w   r1,@(REGPC,r0)

              jsr     @_getNextTask          ; get the next task with highest priority in readyqueue
              jsr     @_dispatch             ; dispatch highest priority task

              pop     r1                    ; remove the old content of the stack and

;Return value from dispatch (in register r0) have two purpose.
;   If bit 0 is set a taskswitch must be performed
;   If bit 0 is cleared, no taskswitch is to be done
;
;   If bit 1 is set, the start address of t_return is pushed on stack
;   It bit 1 is cleared, no action taken
;
;   We use bit 1 in the return value to control if the start address must
;   be pushed on the stack or not. By doing this we eliminate the need
;   of a explicit call to t_return() in user tasks.

              btst    #1,r0l
              beq     nopush_t_return3
              mov.w   #_hal_t_return,r0
              push    r0
nopush_t_return3:
              mov.w   @_curr_disp,r0
              mov.w   @(REGPC,r0),r1            ; replace it with new return address
              push    r1

              ;restore all registers to what is recorded in the dispatch_struct.
              mov.w   @(REG6,r0),r6
              mov.w   @(REG5,r0),r5
              mov.w   @(REG4,r0),r4
              mov.w   @(REG3,r0),r3
              mov.w   @(REG2,r0),r2
              mov.w   @(REG1,r0),r1
              mov.w   @(REG0,r0),r0     ; Registers are set to the new tasks    
         
              andc    #0x7f,ccr     // enable interrupts
              rts
              
/**************************************************************
  End of YIELD_TASKSWITCH module
**************************************************************/
              .END
              