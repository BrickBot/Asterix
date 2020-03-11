/**************************************************************
    ##################################################
        ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################

    Kernel.c

    Basic kernel functionality.
    Most of the functions included in the kernel.

    Calls come from: timer tick, t_return, and yield.

    Date:     2001-03-22
    Authors:  Henrik Thane, Daniel Sundmark,
              Anders Petterson, Andreas Engberg

**************************************************************/

/***************************************************************
  Includes
 **************************************************************/
#include <stdlib.h>
#include <os_kernel.h>


/* Just for auxilirary output, remove */
#include <rcx_display.h>

/***************************************************************
  Defines
'**************************************************************/

/***************************************************************
  Function prototypes
 **************************************************************/
/* Locally known functions */
static void moveToExec( taskid_t taskid );



/***************************************************************
  Global variables
 **************************************************************/
TCB_t*      curr_tcb;
dispatch_t* curr_disp;

taskid_t    nexttask        = -1;
taskindex_t ExecTask        = 0;
ticks_t     ticks           = -1;

uint16      lasterror       = 0;

unsigned char *firmware_string = "Do you byte, when I knock?"; /* Extra HW Specific */


/***************************************************************
  External variables
 **************************************************************/
extern uint16 default_mode;


#ifdef  __TIME_MEASURE__


#include <rcx_display.h>
#include <hal_common_interrupt_func.h>
#include <rcx_button.h>

/***************************************************************
  void main( void )

  Start the kernel
***************************************************************/
int main( void )
{
  uint16 i,j;
  int16 timerticks=0;
  int16 button;


   /*-----------------------------------------------------------
     Change to the default mode and start Asterix RTOS.
    -----------------------------------------------------------*/
    os_changeMode( default_mode );
    hal_stopTimer();

    ticks=0;
    hal_initTimer(hal_timemeasure);
    hal_startTimer();

    TCBList[measuretask].startAdress(TCBList[measuretask].param);

    hal_stopTimer();
    timerticks = (int16)*((int16 *)0xff92);
/* Skriv ut vad timern innehåller och/eller ticks */


    outputLCD(ticks);

    do
    {
      readButtons( &button );
    }while( !(button & VIEW_BUTTON) );

  /* Wait until the VIEW button is released */
    do
    {
      readButtons( &button );
    }while( (button & VIEW_BUTTON) );


    printHex( timerticks);
    refreshDisplay();

    do
    {
      readButtons( &button );
    }while( !(button & VIEW_BUTTON) );

  /* Wait until the VIEW button is released */
    do
    {
      readButtons( &button );
    }while( (button & VIEW_BUTTON) );


    reset();

}

#else // THE REAL STUFF, NO TIME MEASUREMENTS

/***************************************************************
  void main( void )

  Start the kernel
***************************************************************/
int main( void )
{


        /*-----------------------------------------------------------
            Disable interupts
        -----------------------------------------------------------*/
        hal_disable_interrupt();
        hal_stopTimer();


        /*-----------------------------------------------------------
            Initialize the Asterix system stack. Asterix uses a single
            stack. It is possible since all tasks are single-shot tasks,
            i.e., are of terminating character
        -----------------------------------------------------------*/
        hal_initStack( &stack_mem[stacksize-1] );



#ifdef __IRQ__

        /*-----------------------------------------------------------
            Initialize the interrupt vectors.
        -----------------------------------------------------------*/
        hal_setIRQ0();
        hal_setIRQ1();

#endif  /* __IRQ__ */



        /*-----------------------------------------------------------
            Set the timer to the right frequency. The right frequency
            is calculated from a user defined value from the
            configuration file.
        -----------------------------------------------------------*/
        hal_initTimer(hal_timertick);

        /*-----------------------------------------------------------
            Call the user initiliazing routine. In this routine, the user
            can put initializing that should be run only once.
        -----------------------------------------------------------*/
        userinit();

        /*-----------------------------------------------------------
            Clear all general purpose CPU register (r0-r6)
        -----------------------------------------------------------*/
        hal_clearRegisters();


        /*-----------------------------------------------------------
            Change to the default mode and start Asterix RTOS.
        -----------------------------------------------------------*/
	os_changeMode( default_mode );

        /*-----------------------------------------------------------
            Initialize the lists of Task Control Blocks.
	    Must be done in os_changeMode!
        -----------------------------------------------------------*/
	/*
	  curr_tcb  = TCBList;
	  curr_disp = displist;
	*/

        /*-----------------------------------------------------------
            Start the Asterix RTOS
	    This hve been moved to os_changeMode
        -----------------------------------------------------------*/
	//	hal_startTimer();
	//	hal_enable_interrupt();

        /*-----------------------------------------------------------
            Call the background task - the Idle task.
        -----------------------------------------------------------*/
        while(1) idletask(NULL);
}
#endif


/***************************************************************
  void getNextTask(void)

  Find the task with the highest priority in the readyqueue and
  compare it with the priority of the executing task. Determine
  whether a taskswitch is necessary.
***************************************************************/
void getNextTask(void)
{
    uint16          i=0;
    volatile uint16 dummy = 0;        /* Dummy for jitter minimization ( Check semantics of volatile) */
    uint16          max_prio=0;       /* Deafult priority of idle task */


        nexttask  = 0;                        /* Default index to idle task */

        /*----------------------------------------------
            Loop through the ready queue and pick the
            task with the highest priority
        ----------------------------------------------*/
        for( i=1 ; i < NRTCBS ; i++ )
        {

            if( (displist[i].STATE == READY) && (displist[i].current_priority > max_prio) )
            {
                max_prio = displist[i].current_priority;
                nexttask = i;
            }
            else
            {
                dummy = displist[i].current_priority;
                dummy = i;

            }
        }

        /*----------------------------------------------
            Check if the task in ready has a higher priority
            than the executing task. If the executing task
            has terminated a call to hal_t_return has been
            performed and the state of the executing task
            has been set to WAITING.
        ----------------------------------------------*/
        if (    (displist[nexttask].current_priority
                < displist[ExecTask].current_priority)
                &&  displist[ExecTask].STATE == RUNNING)
        {
            /*----------------------------------------------
                No! Executing task still rules!
            ---------------------------------------------*/
            nexttask = ExecTask;
        }
        else
        {
            /*----------------------------------------------
                The hi prio task in ready has higher prio
                than the executing task or the executing task
                has terminated. This will lead to
                a task switch.
            ----------------------------------------------*/
        }
}

/***************************************************************
  void moveToReady( taskid_t taskid )

  Set the state of taskid in the dispatch list to READY
'**************************************************************/
void moveToReady( taskid_t taskid )
{
    displist[taskid].STATE = READY;

}


/***************************************************************
  void moveToExec( taskid_t taskid )

  Set the state of taskid in the dispatch list to RUNNING. Set
  ExecTask to taskid.
'**************************************************************/
void moveToExec( taskid_t taskid )
{
    displist[taskid].STATE = RUNNING;
    ExecTask = taskid;
}


/***************************************************************
  void moveToWaiting( taskid_t taskid )

  Set the state of taskid in the dispatch list to WAITING
***************************************************************/
void moveToWaiting( taskid_t taskid )
{
    if(TCBList[taskid].T == 0)
        displist[taskid].STATE = SIG_BLOCKED;
    else
        displist[taskid].STATE = WAITING;
}



/***************************************************************
  void moveToWaiting( taskid_t taskid )

  At each system tick update in tasks TCB entry tick left to
  deadline(LDL). Perform control of the tasks deadline. Initate
  and move task with new period to the readyqueue.
***************************************************************/
void updateTCBList( void )
{
    taskindex_t taskidx=1;

    for( taskidx = 1 ; taskidx < NRTASKS; taskidx++ ){

      switch( displist[ taskidx ].STATE ){
        case READY:
        case RUNNING:
          if( displist[ taskidx ].LDL == MISSEDDEADLINE ){
            TCBList[ taskidx ].errorHandler( ERROR_MISSEDDEADLINE );
          }

          displist[ taskidx ].LDL--;
          break;
        case WAITING:

          if( displist[ taskidx ].LDP == 0 ){

            initTask( taskidx );
            moveToReady( taskidx );

#ifdef __WAITFREE__
            updateWaitfree(taskidx);
#endif

          }

          break;
      case SIG_BLOCKED:
        if( TCBList[ taskidx ].T != 0 ){
          if( displist[ taskidx ].LDL == MISSEDDEADLINE )
            TCBList[ taskidx ].errorHandler( ERROR_MISSEDDEADLINE );
          displist[ taskidx ].LDL--;
        }
        break;

      }


      displist[ taskidx ].LDP--;
    }

    ticks++;
}

/***************************************************************
  int dispatch(void)

  If the task to run during the next tick is different from
  the currently executing task then make a task switch.

  Returns:
            0 = No task switch. Executing task continues
            1 = Task switch
            3 = Task switch and push hal_t_return on stack.
                By doing this we eliminate the need of a explicit
                call to hal_t_return() in user tasks.
***************************************************************/
int dispatch(void)
{
    int return_val  = 0;

    /*----------------------------------------------
    Check if there should be a task switch.
    ----------------------------------------------*/
    if(nexttask == ExecTask)
    {
        /*----------------------------------------------
        No task switch. The currently executing task
        has the highest priority and has not terminated.
        ----------------------------------------------*/
        return_val = 0;

        /*----------------------------------------------
        No task switch. The currently executing task
        has the highest priority and has not terminated.
        ----------------------------------------------*/

    }
    else
    {
        /*----------------------------------------------
        TASK SWITCH! A task in ready has higher priority
        than the executing one, or the executing one has
        terminated.
        ----------------------------------------------*/
        if (displist[ExecTask].STATE == WAITING || displist[ExecTask].STATE == SIG_BLOCKED)
        {
          /*----------------------------------------------
          The executing task has terminated and has
          already been moved to WAITING or it is a aperiodic
          task triggered by a signal.
          ----------------------------------------------*/
        }
        else
        {
          /*----------------------------------------------
          This is a genuine task switch. Move the currently
          executing task to READY.
          ----------------------------------------------*/
          moveToReady( ExecTask );
        }

        /*----------------------------------------------
        Move the higest priority task in the ready queue
        to executing (= RUNNING).
        ----------------------------------------------*/
        curr_tcb    =   &TCBList[nexttask];
        curr_disp   =   &displist[nexttask];
        moveToExec( nexttask );

        /*----------------------------------------------
        Check if the new executing task has been
        preempted previously. This is important to know
        since we push the return address to
        "hal_t_return" on the stack the first time the
        task is executed. This allows the tasks to just
        make a regular return() when terminating. (this
        excludes the idle task however)
        ----------------------------------------------*/
        if( (curr_disp->PC == curr_tcb->startAdress)
            && ( nexttask != IDLETASK))
        {
          /*----------------------------------------------
          Push "hal_t_return".
          ----------------------------------------------*/
          return_val = 3;
        }
        else
        {
          /*----------------------------------------------
          Plain old task switch.
          ----------------------------------------------*/
          return_val = 1;
        }
    }

    return(return_val);
}


/***************************************************************
    void initTask( taskid_t taskid )

    Arguments:  The task identity.
    Returns:    zip
    Purpose:    Sets  the entry of displist to the initial values
***************************************************************/
void initTask( taskid_t taskid )
{
    TCB_t       *tcbtemp=&TCBList[taskid];
    dispatch_t  *disptemp=&displist[taskid];



    if(taskid==0)
        return ;
    if(tcbtemp->T == 0)
        disptemp->STATE  =   SIG_BLOCKED;
    else
        disptemp->STATE  =   WAITING;

    disptemp->PC    =   (programcounter_t)(tcbtemp->startAdress);
    disptemp->GPR0  =   (cpureg_t)(tcbtemp->param);
    disptemp->GPR1  =   (cpureg_t)0;
    disptemp->GPR2  =   (cpureg_t)0;
    disptemp->GPR3  =   (cpureg_t)0;
    disptemp->GPR4  =   (cpureg_t)0;
    disptemp->GPR5  =   (cpureg_t)0;
    disptemp->GPR6  =   (cpureg_t)0;
    disptemp->CCR   =   tcbtemp->ConditionCodeReg;
    disptemp->LDL   =   tcbtemp->DL - tcbtemp->O - 1 ;
    disptemp->LDP   =   tcbtemp->T - 1;

}

/**************************************************************
  ticks_t getTicks(void)


  Usage       : getTicks();
  Arguments   : none
  Returns     : value in timer

  Purpose     : Reads the system-timer

**************************************************************/
ticks_t getTicks(void)
{
   return ticks;
}





/**************************************************************
  void  os_changeMode( uint16 mode )


  Usage       : os_changeMode( ModeName );
  Arguments   : the name of the mode given in the 
                configuration file (obelic.conf).
  Returns     : -
  Purpose     : Initialize and switch to another mode

**************************************************************/
void os_changeMode( uint16 mode)
{

        hal_disable_interrupt();
	hal_stopTimer();


	curr_mode = mode;

	NRTCBS = no_tasks_arr[mode];
	NRTASKS = no_tasks_arr[mode];

	TCBList = TCBList_arr[mode];
	displist = displist_arr[mode];

#ifdef __SIGNALS__
	no_signals =  no_sig_arr[mode];
	signals = signals_arr[mode];
#endif /* __SIGNALS__ */

#ifdef __SEMAPHORE__
	no_semaphores = no_sem_arr[mode];
	semaphore = semaphore_arr[mode];
#endif /* __SEMAPHORES__ */

#ifdef __WAITFREE__
	no_waitfree = no_wf_arr[mode];
	buffer = buffer_arr[mode];
#endif /* __WAITFREE__ */

	timerVal = timerVal_arr[mode];

        /*-----------------------------------------------------------
	  Initialize the lists of Task Control Blocks.
	-----------------------------------------------------------*/
	curr_tcb  = TCBList;
	curr_disp = displist;

	hal_initTimer(hal_timertick);

	hal_enable_interrupt();
	hal_startTimer();

	return;
}
