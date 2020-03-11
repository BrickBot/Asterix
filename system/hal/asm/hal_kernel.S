;/**************************************************************
;   ##################################################
;       ASTERIX RTOS                                
;       COPYRIGHT (c) 2000, 2001                    
;   ##################################################
;
;   Hardware Adaption Layer (HAL)
;   Hitatchi H8/300
;
;   Kernel related functions that are target cpu specific  
;
;   File:     hal_kernel.s
;   Date:     2001-03-08
;   Authors:  Henrik Thane, Daniel Sundmark, 
;             Anders Petterson, Andreas Engberg 
;
;   Change log: Last in file.
; **************************************************************/

#include <os_tasks_inc.h>

;/***************************************************************
;  External functions
; **************************************************************/


;/***************************************************************
;  Function prototypes
; **************************************************************/

        .GLOBAL     _hal_initStack
        .GLOBAL     _hal_restoreStack
        .GLOBAL     _hal_clearRegisters
        .GLOBAL     _hal_timemeasure  



;/***************************************************************
;  Defines 
; **************************************************************/


;/***************************************************************
;  Inlcudes
; **************************************************************/


;/**************************************************************
;  The KERNEL module
; *************************************************************/
        .NAME   KERNEL
              
; /* Tell the assembler that this belong to the code segment */
        .section    .text 

;/**************************************************************
;  void hal_initStack(stack_t *stk)
;
;  Initiate the system stack used by all tasks and the
;  kernel.
;**************************************************************/
_hal_initStack:
              pop   r6
              mov.w sp,r5
              mov.w r0,sp
              push  r5
              push  r6
              rts

/**************************************************************
  void hal_restoreStack(void)

  Restore the stack inorder to gracefully exit to 
  the operating system.

**************************************************************/
_hal_restoreStack:
              pop   sp
              rts

/**************************************************************
  void hal_clearRegister(void)

  Clear all the general purpose CPU registers. This is required 
  by the deterministic replay debugger.

**************************************************************/
_hal_clearRegisters:
              mov.w #0,r0
              mov.w r0,r1
              mov.w r0,r2
              mov.w r0,r3
              mov.w r0,r4
              mov.w r0,r5
              mov.w r0,r6
              rts

/**************************************************************
  void hal_timemeasure( void );


**************************************************************/
#ifdef __TIME_MEASURE__
_hal_timemeasure:  
                  push   r0
                  push   r1
                  mov.w  #0xff91,r0
                  bclr.b #0x03,@r0
                  mov.w  @_ticks,r0
                  mov.w  #1,r1
                  add.w  r1,r0
                  mov.w  r0,@_ticks
                  pop    r1
                  pop    r0
                  rts

#endif /* __TIME_MEASURE__ */



/**************************************************************
  End of KERNEL module
**************************************************************/              
              .END
