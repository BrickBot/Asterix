/*
##################################################
##  ASTERIX PROJECT                             ##
##                                              ##
## Subproject   : Asterix, realtime kernel      ##
##                                              ##
## COPYRIGHT (c) 2000                           ##
##################################################
*/
/*
**************************************************
**  File        : irqISR.c                      **
**  Date        : 2000-05-31                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--    These functions are used as a part of the --
--    the interrupt handling. The functions are --
--    scheduled as user tasks, therefore they   --
--    must be terminated as ordinary tasks.     --
--                                              --
--------------------------------------------------
*/


#include <os_kernel.h>
#include <rcx_display.h>
/*
**************************************************
**  Name        : void irq0_isr( void* param )  **
**  Usage       :                               **
**      Must be setuped as a signal and it must **
**      be triggered by the signal number that  **
**      is set in the irq0 handler.             **
**                                              **
**  Arguments   : Pointer to a mamory location  **
**                or a NULL-pointer.            **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : To be able to schedule an irq **
**                as a user task. IRQ 0 takes   **
**                care of interrputs caused by  **
**                the run button.               **
**                Hardware specific, this       **
**                version is for LegoMindstorm  **
**                                              **
**  Use         :                               **
**                                              **
**************************************************
*/

C_task void irq0_isr( void *ignore )
{


}

/*
**************************************************
**  Name        : void irq1_isr( void* param )  **
**  Usage       :                               **
**      Must be setuped as a signal and it must **
**      be triggered by the signal number that  **
**      is set in the irq1 handler.             **
**                                              **
**  Arguments   : Pointer to a mamory location  **
**                or a NULL-pointer.            **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : To be able to schedule an irq **
**                as a user task. IRQ 1 takes   **
**                care of interrputs caused by  **
**                on/off button.                **
**                Hardware specific, this       **
**                version is for LegoMindstorm  **
**                                              **
**  Use         :                               **
**                                              **
**************************************************
*/
C_task void irq1_isr( void *ignore )
{


    *(char *)0xFFCC = 1;
    (*(void (**)(void))0x0000)();

}


/*
**************************************************
**  Name        : void irqerror( uint16 error)  **
**  Usage       :                               **
**    initialization:                           **
**        TCBList[idx].errorHandler=irqerror(); **
**    calling syntax:                           **
**        TCBList[idx].errorHandler(ERRORCODE); **
**                                              **
**  Arguments   : An errortype, this must be an **
**                positive integer in the range **
**                from 1 to 9999                **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : Errorhandler used by          **
**                IRQ-handlers                  **
**                                              **
**  Use         : stopTimer();                  **
**                outputLCD();                  **
**                                              **
**************************************************
*/
C_task void irqerror( uint16 errorcode )
{
  hal_stopTimer();

    outputLCD( errorcode );
    while(1);

}

