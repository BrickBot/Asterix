/**************************************************************
    ##################################################
        ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################
  Hardware Adaption Layer (HAL)
  Hitatchi H8/300

  Assembler and hardware specific definitions (IAR specific)

  File:     hal_assembler.h
  Date:     2001-03-09
  Authors:  Henrik Thane, Daniel Sundmark

**************************************************************/
#ifndef __HAL_ASM_H__
#define __HAL_ASM_H__


/***************************************************************
  External variables
 **************************************************************/

        .EXTERN     ExecTask
        .EXTERN     curr_disp
        .EXTERN     register0_temp
        .EXTERN     register6_temp
        .EXTERN     programcounter_temp

/***************************************************************
  Includes
**************************************************************/

/***************************************************************
  Defines
**************************************************************/

        #define R_0          0x00
        #define R_1          0x02
        #define R_2          0x04
        #define R_3          0x06
        #define R_4          0x08
        #define R_5          0x0a
        #define R_6          0x0c

       /*-----------------------------------------------------------
          Getting things off the stack by these offsets
       -----------------------------------------------------------*/
        #define R6_STACK   0x02
        #define CCR_STACK     0x04
        #define PC_STACK      0x06

       /*-----------------------------------------------------------
          Offsets to entrys in the TCB struct
       -----------------------------------------------------------*/
        #define STATE         0x00
        #define LDL           0x02
        #define REGPC         0x04
        #define REGCCR        0x06
        #define REG0          0x08
        #define REG1          0x0a
        #define REG2          0x0c
        #define REG3          0x0e
        #define REG4          0x10
        #define REG5          0x12
        #define REG6          0x14
        #define WFBUFPOINTER  0x1a

#endif  /* __HAL_ASM_H__ */
