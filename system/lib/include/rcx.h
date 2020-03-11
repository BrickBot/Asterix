/**************************************************************
  Hardware Adaption Layer (HAL)
  Hitatchi H8/300

  Definitions of RCX memory area offsets

  File:     rcx.h
  Date:     2001-03-15
  Authors:  Henrik Thane, Daniel Sundmark

**************************************************************/

#ifndef __RCX_H__
#define __RCX_H__

/***************************************************************
  Includes 
***************************************************************/
#include <rcx_sensor.h>
#include <rcx_display.h>
#include <rcx_serial.h>
#include <rcx_datalink.h>
#include <rcx_motor.h>
#include <rcx_button.h>
#include <rcx_misc.h>
/***************************************************************
  Defines
***************************************************************/
#define   T_IER       *((unsigned char*)0xff90)
#define   T_CSR       *((unsigned char*)0xff91)
#define   T_CNT       *((unsigned char*)0xff92)
#define   T_OCRA      *((unsigned char*)0xff94)
#define   T_OCRB      *((unsigned char*)0xff94)
#define   T_CR        *((unsigned char*)0xff96)
#define   T_OCR       *((unsigned char*)0xff97)
#define   T_ICRA      *((unsigned char*)0xff98)
#define   T_ICRB      *((unsigned char*)0xff9a)
#define   T_ICRC      *((unsigned char*)0xff9c)
#define   T_ICRD      *((unsigned char*)0xff9e)
#define   PORT1_PCR   *((unsigned char*)0xffac)
#define   PORT2_PCR   *((unsigned char*)0xffad)
#define   PORT3_PCR   *((unsigned char*)0xffae)
#define   PORT1_DDR   *((unsigned char*)0xffb0)
#define   PORT2_DDR   *((unsigned char*)0xffb1)
#define   PORT1       *((unsigned char*)0xffb2)
#define   PORT2       *((unsigned char*)0xffb3)
#define   PORT3_DDR   *((unsigned char*)0xffb4)
#define   PORT4_DDR   *((unsigned char*)0xffb5)
#define   PORT3       *((unsigned char*)0xffb6)
#define   PORT4       *((volatile unsigned char*)0xffb7)
#define   PORT5_DDR   *((unsigned char*)0xffb8)
#define   PORT6_DDR   *((volatile unsigned char*)0xffb9)
#define   PORT5       *((volatile unsigned char*)0xffba)
#define   PORT6       *((volatile unsigned char*)0xffbb)
#define   PORT7       *((unsigned char*)0xffbe)
#define   STCR        *((unsigned char*)0xffc3)
#define   SYSCR       *((unsigned char*)0xffc4)
#define   T0_CR       *((unsigned char*)0xffc8)
#define   T0_CSR      *((unsigned char*)0xffc9)
#define   T0_CORA     *((unsigned char*)0xffca)
#define   T0_CORB     *((unsigned char*)0xffcb)
#define   T0_CNT      *((unsigned char*)0xffcc)
#define   T1_CR       *((unsigned char*)0xffd0)
#define   T1_CSR      *((volatile unsigned char*)0xffd1)
#define   T1_CORA     *((unsigned char*)0xffd2)
#define   T1_CORB     *((unsigned char*)0xffd3)
#define   T1_CNT      *((volatile unsigned char*)0xffd4)
#define   S_MR        *((unsigned char*)0xffd8)
#define   S_BRR       *((unsigned char*)0xffd9)
#define   S_CR        *((unsigned char*)0xffda)
#define   S_TDR       *((unsigned char*)0xffdb)
#define   S_SR        *((volatile unsigned char*)0xffdc)
#define   S_RDR       *((volatile unsigned char*)0xffdd)
#define   AD_A        *((unsigned char*)0xffe0)
#define   AD_A_H      *((unsigned char*)0xffe0)
#define   AD_A_L      *((unsigned char*)0xffe1)
#define   AD_B        *((unsigned char*)0xffe2)
#define   AD_B_H      *((unsigned char*)0xffe2)
#define   AD_B_L      *((unsigned char*)0xffe3)
#define   AD_C        *((unsigned char*)0xffe4)
#define   AD_C_H      *((unsigned char*)0xffe4)
#define   AD_C_L      *((unsigned char*)0xffe5)
#define   AD_D        *((unsigned char*)0xffe6)
#define   AD_D_H      *((unsigned char*)0xffe6)
#define   AD_D_L      *((unsigned char*)0xffe7)
#define   AD_CSR      *((unsigned char*)0xffe8)
#define   AD_CR       *((unsigned char*)0xffe9)


#endif
