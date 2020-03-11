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
**  File        : user.c                        **
**  Date        : 2005-08-10                    **
**  Author(s)   : Anders Pettersson             **
**                                              **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--      User defined tasks can be placed here.  --
--      The idle task must always be present.   --
--                                              --
--------------------------------------------------
*/

/* System specific includes do not remove */
#include <os_kernel.h>
#include <sys_spec.h>

/* User includes can be placed here */
#include <rcx_display.h>


/*
----------------------------------------
-- Idle task, do not remove or put in --
-- non-terminating loops.             --
----------------------------------------
*/
C_task void idletask( void *ignore )
{

}


/*
----------------------------------------
-- Example task: Increasing a counter --
-- and write it out on the LCD-display--
----------------------------------------
*/
C_task void task_a ( void *ignore )
{
  static uint16 counter=0;

  outputLCD( counter++ );
}
