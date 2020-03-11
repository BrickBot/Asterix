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
**  File        : axlib.c                       **
**  Date        : 2000-05-10                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--    Collection of the most common systemcalls.--
--                                              --
--                                              --
--                                              --
--------------------------------------------------
*/

#include <os_tasks.h>

extern taskid_t ExecTask;
/*
**************************************************
**  Name        : taskid_t self( void )         **
**  Usage       : taskid = self();              **
**  Arguments   : none                          **
**  Returns     : The task identity             **
**                                              **
**  Purpose     :  Let the task ask for his own **
**                 identity.                    **
**                                              **
**  Use         : none                          **
**************************************************
*/
taskid_t self( void )
{
        return ExecTask;
}

