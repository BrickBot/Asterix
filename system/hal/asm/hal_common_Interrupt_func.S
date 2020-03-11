;/**************************************************************
;   ##################################################
;       ASTERIX RTOS                                
;       COPYRIGHT (c) 2000, 2001                    
;   ##################################################
;
;   Hardware Adaption Layer (HAL)
;   Hitatchi H8/300
;    
;   Interrupt related functions  
;    
;   File:     hal_common_Interrupt_func.s
;   Date:     2001-03-08
;   Authors:  Henrik Thane (HT), Daniel Sundmark (DS),
;             Anders Pettersson (AP)
;
;   Change log: Last in file.
;**************************************************************/

;/*
;**************************************************
;** There are six functions in this code section **
;** all are inline assembler.                    **
;**                                              **
;**  Name        : void irq0()                   **
;**                void irq1()                   **
;**                void setirq0()                **
;**                void setirq1()                **
;**                void storeRegisters()         **
;**                void retreiveRegisters()      **
;**                                              **
;**  Usage       : IRQ0vector = irq0;            **
;**                IRQ1vector = irq1;            **
;**                setirq0();                    **
;**                setirq1();                    **
;**                storeRegisters();             **
;**                retreiveRegisters();          **
;**                                              **
;**  Arguments   : none                          **
;**                                              **
;**  Purpose     : To reserve IRQ so the user    **
;**                not have access to the IRQ.   **
;**                This because IRQ have higher  **
;**                priority than the timer.      **
;**                                              **
;**  Use         :                               **
;**         irq0          invokeISR();           **
;**         irq1          invokeISR();           **
;**                                              **
;**************************************************
;*/


;/***************************************************************
;   Externals
;**************************************************************/
        .EXTERN     _invokeISR



;/***************************************************************
;   Function prototypes
;**************************************************************/
        .GLOBAL     _hal_disable_interrupt
        .GLOBAL     _hal_enable_interrupt
        .GLOBAL     _hal_irq0
        .GLOBAL     _hal_irq1
        .GLOBAL     _hal_setIRQ0
        .GLOBAL     _hal_setIRQ1
        .GLOBAL     _hal_storeRegisters
        .GLOBAL     _hal_retrieveRegisters



;/***************************************************************
;   Includes
;**************************************************************/
;/* #include <os_tasks.h> */
#include <hal_assembler.h>
#include <os_tasks_inc.h>


;/***************************************************************
;   Defines
;**************************************************************/

;/**************************************************************
;   The COMMON_INTERRUPT_FUNCTIONS module
;**************************************************************/
        .NAME       COMMON_INTERRUPT_FUNCTIONS

; /* Tell the assembler that this belong to the code segment */
    .section        .text

;/**************************************************************
;   Disable interrupt
;**************************************************************/
_hal_disable_interrupt:    orc     #0x80,ccr       //disable the damned interrupt
                          rts

                          
/**************************************************************
  Enable interrupt
**************************************************************/
_hal_enable_interrupt:     andc    #0x7f,ccr       //enable the damned interrupt
                          rts

                          
/***********************************************************************
  irq0

  This is the hardware interrupt service routine for interrupt 0.
***********************************************************************/
_hal_irq0:

          /* MÅSTE ACKA INTERRUPTEN HÄR???? */



                pop     r6                      /* remove return address to rcx rom from top of stack */
                pop     r6                      /* pop r6 from stack - rcx rom put it there */

                jsr     @_hal_storeRegisters
                mov.w   #0,r0
                jsr     @_invokeISR
                jsr     @_hal_retrieveRegisters
                rte


/***********************************************************************
  hal_irq1()

  This is the hardware interrupt service routine for interrupt 0.
***********************************************************************/
_hal_irq1:

          /* MÅSTE ACKA INTERRUPTEN HÄR????? */
          


                pop     r6                      // remove return address to rcx rom from top of stack
                pop     r6                      // pop r6 from stack - rcx rom put it there 

                jsr     @_hal_storeRegisters
                mov.w   #1,r0
                jsr     @_invokeISR
                jsr     @_hal_retrieveRegisters
                rte

/***********************************************************************
  hal_setIRQ0()

  Initiate the IRQ 0 interrupt vector with a reference to _irq0
***********************************************************************/
_hal_setIRQ0:          

              mov.w   #0xfd94,r0      // IRQ0 Interrupt vector
              mov.w   #_hal_irq0,r1
              mov.w   r1,@r0

              mov.w   #0xffc7,r0      // Enable IRQ0 interrupt
              bset.b  #0,@r0          // Set bit 0 in IER

              mov.w   #0xffc6,r0      // Set falling edge to activator
              bset.b  #0,@r0          // Set bit 0 in ISCR
              rts


/***********************************************************************
  hal_setIRQ1()

  Initiate the IRQ 1 interrupt vector with a reference to _irq1
***********************************************************************/
_hal_setIRQ1:

              mov.w   #0xfd96,r0      /* IRQ1 Interrupt vector */
              mov.w   #_hal_irq1,r1
              mov.w   r1,@r0

              mov.w   #0xffc7,r0      /* Enable IRQ1 interrupt */
              bset.b  #1,@r0          /* Set bit 1 in IER */

              mov.w   #0xffc6,r0      /* Set falling edge to activator */
              bset.b  #1,@r0          /* Set bit 1 in ISCR */

              rts


/***********************************************************************
  hal_storeRegisters()

  Store all registers temporary
***********************************************************************/
_hal_storeRegisters:       
                          push    r0              /* Store r0 on stack */
                          mov.w   @_tmpregAdr,r0  /* Move tmpregs base adress to r0 */
                          mov.w   r1,@(R_1,r0)
                          pop     r1
                          mov.w   r1,@(R_0,r0)
                          mov.w   r2,@(R_2,r0)
                          mov.w   r3,@(R_3,r0)
                          mov.w   r4,@(R_4,r0)
                          mov.w   r5,@(R_5,r0)
                          mov.w   r6,@(R_6,r0)     /* Store all registers in tmpregs buffer */
                          rts


                          
/***********************************************************************
  hal_retrieveRegisters()

  Restore all registers from temporary storage
***********************************************************************/
_hal_retrieveRegisters:    
                          mov.w   @_tmpregAdr,r0  /* Move tmpregs base adress to r0 */
                          mov.w   @(R_6,r0),r6
                          mov.w   @(R_5,r0),r5
                          mov.w   @(R_4,r0),r4
                          mov.w   @(R_3,r0),r3
                          mov.w   @(R_2,r0),r2
                          mov.w   @(R_1,r0),r1
                          mov.w   @(R_0,r0),r0     /* Restore all registers */
                          rts


/**************************************************************
  End of COMMON_INTERRUPT_FUNCTIONS module
**************************************************************/
                          .END
