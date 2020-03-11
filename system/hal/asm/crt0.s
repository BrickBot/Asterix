;/**************************************************************
;   ##################################################
;       ASTERIX RTOS                                
;       COPYRIGHT (c) 2000, 2001                    
;   ##################################################
;
;   Hardware Adaption Layer (HAL)
;   Intel IA32, PC-Clones/PC104 boards
;
;   Startup code for Intel 80x386 processor, works on PC-clones and PC104
;   boards. Should work for all boards with identical hardware architecture as the
;   above  
;
;   File:     crt0.s
;   Date:     2001-08-09
;   Authors:  Anders Petterson (AP) 
;
;   Change log: Last in file.
; **************************************************************/

;/***************************************************************
;  External functions
; **************************************************************/
        .extern _main


;/***************************************************************
;  Function prototypes
; **************************************************************/
        .global _start_crt0


;/***************************************************************
;  Defines 
; **************************************************************/



;/**************************************************************
;  The STARTUP module
;   This must be the first function of the application,
;   the linker set this functions start address as start address
;   of the application. This is controlled by the linker script
;   and parameters passed to the linker on the command line.
; *************************************************************/
        .NAME   STARTUP
              
; /* Tell the linker that this belong to the code segment, and that it is 16bits code */
        .section    .text 

_start_crt0:
                jmp     _main

        .END
        