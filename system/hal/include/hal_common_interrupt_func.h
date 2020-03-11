/**************************************************************
    ##################################################
        ASTERIX RTOS                                
        COPYRIGHT (c) 2000, 2001                    
    ##################################################

  Hardware adaption layer 
  Hitatchi H8/300

  Interrupt related functions  

  File:     hal_common_Interrupt_func.h
  Date:     2001-03-08
  Authors:  Henrik Thane, Daniel Sundmark

**************************************************************/
#ifndef __HAL_INT_REL_FUNC
#define __HAL_INT_REL_FUNC

/**************************************************************
  Includes
**************************************************************/
#include <hal_kertypes.h>


/**************************************************************
  Disable interrupt
**************************************************************/
extern void  hal_disable_interrupt(void);
                          
/**************************************************************
  Enable interrupt
**************************************************************/
extern void  hal_enable_interrupt(void);

/**************************************************************
  Enable interrupt
**************************************************************/
extern void  hal_enable_interrupt(void);

/***********************************************************************
  irq0
  This is the hardware interrupt service routine for interrupt 0.
***********************************************************************/
void hal_irq0(void);

/***********************************************************************
  irq1
  This is the hardware interrupt service routine for interrupt 1.
***********************************************************************/
void    hal_irq1(void);                   

void    hal_setIRQ0(void);                
void    hal_setIRQ1(void);                
void    hal_storeRegisters(void);
void    hal_retreiveRegisters(void);

void    hal_initStack(stack_t *stk);
void    hal_restoreStack(void);
void    hal_clearRegister(void);
void    hal_timemeasure( void );
ticks_t hal_getFRCval(void); 
 
 
 
    
                          
#endif //__HAL_INT_REL_FUNC                 