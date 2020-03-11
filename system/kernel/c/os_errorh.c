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
**  File    : os_errorH.c                       **
**  Date    : 2000-02-27                        **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--   errorh() is the default error/exception    --
--   handler. The handler printouts a errorcode --
--   and halts the system.                      --
--                                              --
--------------------------------------------------
*/
#include <os_kernel.h>
#include <sys_spec.h>

#include <rcx_button.h>
#include <rcx_display.h>

extern void hal_disable_interrupt(void);
extern void hal_enable_interrupt(void);

/*
**************************************************
**  Name        : void errorH(uint16 errorcode) **
**  Usage       :                               **
**        TCBList[idx].errorHandler=errorH();   **
**                                              **
**        TCBList[idx].errorHandler(ERRORCODE); **
**                                              **
**  Arguments   : An errortype, this must be an **
**                positive integer in the range **
**                from 1 to 9999                **
**                                              **
**  Returns     : none                          **
**                                              **
**  Purpose     : Serve as a default error      **
**                handler if the user do not    **
**                define his own                **
**                                              **
**  Use         : stopTimer();                  **
**                outputLCD();                  **
**                                              **
**************************************************
*/

void errorH( uint16 errorcode )
{

  uint16 button;

  hal_stopTimer();


  /* RCX specific, should be printf ansi c */
  outputLCD( (int16)errorcode);

  /* Wait until the VIEW button to be pressed */
  do
  {
      readButtons( &button );
  }while( !(button & VIEW_BUTTON) );

  /* Wait until the VIEW button is released */
  do
  {
      readButtons( &button );
  }while( (button & VIEW_BUTTON) );

  /* RCX specific, should be write() */
  outputLCD( (uint16)self() );

  /* Wait until the VIEW button to be pressed */
  do
  {
      readButtons( &button );
  }while( !(button & VIEW_BUTTON) );

  /* Wait until the VIEW button is released */
  do
  {
      readButtons( &button );
  }while( (button & VIEW_BUTTON) );
      ;

  /* RCX specific, should be write() */
  outputLCD( (int16)getTicks());



  /* Wait until the VIEW button to be pressed */
  do
  {
      readButtons( &button );
  }while( !(button & VIEW_BUTTON) );

  /* Wait until the VIEW button is released */
  do
  {
      readButtons( &button );
  }while( (button & VIEW_BUTTON) );
      ;
  /* RCX specific */
  /* The definition of reset should be in HAL */
  /* in that way we can do a hardware specific reset */
  reset();
}
