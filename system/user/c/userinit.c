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
**  Date        : 2000-05-31                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--      User defined setup of the system can be --
--      placed here, for example code that must --
--      be run only once can be put here.       --
--------------------------------------------------
*/
/* System specific includes do not remove */
#include <os_kernel.h>
#include <sys_spec.h>

/* User includes can be placed here */
#include <rcx_sensor.h>


uint16 default_mode;

C_task void userinit(void)
{

  default_mode = mode_1;
  initSensors();

}

