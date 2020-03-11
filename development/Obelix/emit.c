/**************************************************************
    ##################################################
        OBELIX CONFIGURATION TOOL FOR ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################

    emit.c

    Routines that produce c-code to be compiled into a
    Asterix system.

    Date:     2001-03-27
    Authors:  Henrik Thane, Daniel Sundmark,
              Anders Davidsson, Joakim Lindgren,
              Anders Pettersson

**************************************************************/

/***************************************************************
  Includes
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <Obelix.h>
#include <syntree.h>
#include <emit.h>
#include <eutils.h>

/***************************************************************
  Defines
 **************************************************************/
/* This types should be placed in a shared include file, so that
   consistency is achieved.
*/
#define INT "int16" /* not used yet */
#define STACKTYPE           "stack_t"
#define CLOCKTYPE           "uint16"
#define ERRORTYPE           "uint16"
#define SEMAPHORELIST_T     "semaphorelist_t"
#define DEF_ERROR_ROUTINE   "errorH"


/***************************************************************
  Function prototypes
 **************************************************************/

/* File handling */
void initFiles(void);
void closeFiles(void);
void completeFiles();


/* Init functionality */
void initModes(tTree root);



void createResolution(int res);

void createTestModeSetUp(tTree program);


/* Mode control */
void createModes(tTree modes);
void createMode(tTree mode);


/* Task control */
void createTCBList(tTree tasks);
void createTaskInTCBList(tTree task);
void createDispatchList(tTree tasks,tTree semaphore);
void createTaskInDispatchList(tTree task,tTree semaphore);
void buildTaskList(tTree mode, tTree task_list[]);

/* Function prototypes */
void createFunctionProtos(tTree tasks);
void createPrototype(tTree task);


/* Stack handling */
void createStack(tTree tasks);
void calculateStackSpace(tTree task);


/* Resources */
void createSynch(tTree mode);
void createSemaphores(tTree mode, tTree task_list[]);
void createSemUserArray(tTree semaphores, int no, tTree task_list[]);
void createSemaphoreListinTask(tTree semaphores, tTree tasks);
void createSignals(tTree in_sig, tTree task_list[]);
void createSigUserArray(tTree signals, int no, tTree task_list[]);
int  createSigBlockedArray(tTree signals, int no, tTree task_list[]);
void createWaitFree(tTree mode);
void createTaskBufCtrl(tTree wait_free, tTree tasks);
void defineInterrupts(void);


void FPS_Calc_initialise ( tTree tasks );
void fpscalc_initialise_period ( tTree tasks );
void fpscalc_initialise_wcet ( tTree tasks );
void fpscalc_initialise_deadline ( tTree tasks );
void fpscalc_initialise_priority ( tTree tasks );


void create_BOUND_T_files ( tTree tasks );


/***************************************************************
  Global variables
 **************************************************************/



/***************************************************************
  Local variables
 **************************************************************/
static int         stackSize;
static int         old_stackSize;
static FILE        *cFile;
static FILE        *hFile;
static FILE        *incFile;
static FILE        *fpscalcFile;
static FILE        *boundtFile;

static NameListEl  *routine_list;
static NameListEl  *error_routine_list;

static int         no_tasks;
static int         no_signals;
static int         no_semaphores;
static int         no_waitfree;

static int         error_H_flag;
static int         signals_flag;
static int         wf_flag;
static int         sem_flag;

static int         start_hardtasks;

static char        cFileName[1024];
static char        hFileName[1024];
static char        incFileName[1024];
static char        fpscalcFileName[1024];
static char        boundtFileName[1024];

static char        mode_name[256];


static int         resolution;


/****************************************************
   Name:      createFiles()

   Input:     tTree root, a syntax tree

   Output :   -

   Discr:     Generates c-code for the .h file needed
              by the kernel to set up the system.

   Calls:     
              initFiles(),
              createTestModeSetUp(),
	      initModes(),
	      createModes(),
	      completeFiles(),
	      closeFiles(),
	      freeNameList()
*****************************************************/
void createFile(tTree root)
{
    tTree modes = root->Node.Program.mode;

        error_H_flag        = 0;
	signals_flag        = 0;
	sem_flag            = 0;
	wf_flag             = 0;
	routine_list        = NULL;
	error_routine_list  = NULL;

	initFiles();

	createTestModeSetUp(root);
	createModes(modes);
	initModes(root);

	completeFiles();
	closeFiles();
	freeNameList(&routine_list);
	freeNameList(&error_routine_list);

}



/****************************************************
   Name:      initFiles()
   Input:
   Output:    -
   Calls:     -
   Discr:     Open up the files and write the header.
*****************************************************/
void initFiles(void)
{
    time_t now;
    char *axpath;

        /* Get path to Asterix directory from environment variable */
        axpath = getenv("ASTERIX");
        if( !axpath ){
	    fprintf(stderr, "\n\rCould not read asterix environment variable.");
	    fprintf(stderr, "\n\rSet asterix environment variable to your Asterix path.\n\r");
	    exit(EXIT_FAILURE);
	}

	sprintf(cFileName,"%s%s",axpath,CFILENAME);
	sprintf(hFileName,"%s%s",axpath,HFILENAME);
	sprintf(incFileName,"%s%s",axpath,INCFILENAME);
	sprintf(fpscalcFileName,"%s%s",axpath,FPSCALCFILENAME);
	sprintf(boundtFileName,"%s%s",axpath,BOUNDTFILENAME);


	if(!(cFile = fopen(cFileName, "wb"))){
	    fprintf(stderr, "\n\rCould not create c-file %s, exiting...\n\r", cFileName);
	    exit(EXIT_FAILURE);
	}

	if(!(hFile = fopen(hFileName, "wb"))){
	    fprintf(stderr, "\n\rCould not create h-file %s, exiting...\n\r", hFileName);
	    exit(EXIT_FAILURE);
	}

	if(!(incFile = fopen(incFileName, "wb"))){
	    fprintf(stderr, "\n\rCould not create h-file %s, exiting...\n\r", incFileName);
	    exit(EXIT_FAILURE);
	}

	if(!(fpscalcFile = fopen(fpscalcFileName, "wb"))){
	    fprintf(stderr, "\n\rCould not create the FPS-calc dependent file %s, exiting...\n\r", fpscalcFileName);
	    exit(EXIT_FAILURE);
	}


	if(!(boundtFile = fopen(boundtFileName, "wb"))){
	    fprintf(stderr, "\n\rCould not create the BOUND_T file %s, exiting...\n\r", boundtFileName);
	    exit(EXIT_FAILURE);
	}




	time(&now);

	fprintf(cFile, "%s%s%s%s%s%s%s%s%s",
		"/************************************************************",
		"\n   File: ", cFileName,
		"\n   This file was created by the",
		"\n   Obelix Configuration Compiler",
		"\n   ", ctime(&now),
		"   Do not edit!\n",
		"*************************************************************/\n\n");

	fprintf(cFile, "%s",
		"#include <os_tasks.h>\n\n");

	fprintf(hFile, "%s%s%s%s%s%s%s%s%s",
		"/************************************************************",
		"\n   File: ", hFileName,
		"\n   This file was created by the",
		"\n   Obelix Configuration Compiler",
		"\n   ", ctime(&now),
		"   Do not edit!\n",
		"*************************************************************/\n\n");

	fprintf(hFile, "%s%s",
		"#ifndef __TASKS_H__\n",
		"#define __TASKS_H__\n\n");

	fprintf(hFile, "%s%s%s",
		"#include <stdlib.h>\n",
		"#include <sys_spec.h>\n",
		"#include <os_kernel.h>\n\n");


	fprintf(incFile, "%s%s%s%s%s%s%s%s%s",
		"/************************************************************",
		"\n   File: ", incFileName,
		"\n   This file was created by the",
		"\n   Obelix Configuration Compiler",
		"\n   ", ctime(&now),
		"   Do not edit!\n",
		"*************************************************************/\n\n");

	fprintf(incFile, "%s%s",
		"#ifndef __TASKS_INC_H__\n",
		"#define __TASKS_INC_H__\n\n");


	fprintf(fpscalcFile, "%s%s%s%s%s%s%s%s%s",
		"!/************************************************************",
		"\n!   File: ", fpscalcFileName,
		"\n!   This file was created by the",
		"\n!   Obelix Configuration Compiler",
		"\n!   ", ctime(&now),
		"!  Do not edit anything but the computation time!",
		"\n!*************************************************************/\n\n");

	fprintf(fpscalcFile, "%s",
		"system asterix {");

}


/****************************************************
   Name:      createTestModeSetUp()
   Input:     tTree program, a syntax tree
   Output :   -
   Discr:     Causes the system to run in test mode.
   Calls:     -
*****************************************************/
void createTestModeSetUp(tTree program)
{
    if(program->Node.Program.systemMode == TEST_CONST){
        fprintf(incFile,"#define __TIME_MEASURE__\n");

        fprintf(hFile, "%s%s",
		"#define __TIME_MEASURE__\n",
		"extern taskid_t measuretask;\n\n");

        fprintf(cFile, "%s%d%s",
		"\ntaskid_t measuretask = ", 0, ";\n\n");
    }
}



/****************************************************
   Name:      initModes()

   Input:     A pointer to the root of an Obelix syntax tree.

   Output :   -

   Discr:     Set up necessary arrays and varialbels,
              assignes values where necessary and possible.

   Calls:     -
*****************************************************/
void initModes(tTree root)
{
    unsigned int value = 0;

        tTree mode = root->Node.Program.mode;

	fprintf(hFile, "%s%s%s%s%s",
		"/************************************************************\n",
		"   Mode setup\n",
		"   Set up each individual mode and references needed to\n",
		"   access each mode\n",
		"*************************************************************/\n\n");

	fprintf(cFile, "%s%s%s%s%s",
		"/************************************************************\n",
		"   Mode setup\n",
		"   Set up each individual mode and references needed to\n",
		"   access each mode\n",
		"*************************************************************/\n\n");


	fprintf(cFile,"uint16 timerVal;\n\n");
	fprintf(hFile,"extern uint16 timerVal;\n\n");


	fprintf(cFile, "%s%s",
		"uint16 NRTCBS;\n",
		"uint16 NRTASKS;\n\n");

	fprintf(hFile, "%s%s",
		"extern uint16 NRTCBS;\n",
		"extern uint16 NRTASKS;\n\n");


	/* Assign an unique identity to each mode*/
	mode = root->Node.Program.mode;

	while( mode ){
	    fprintf(hFile, "%s%s%s",
		    "extern uint16 ", mode->Node.Mode.name,";\n");
	    fprintf(cFile, "%s%s%s%d;\n",
		    "uint16 ", mode->Node.Mode.name, " = ", value++);
	    mode = mode->Node.Mode.Next;
	}
	fprintf(hFile,"\n");
	fprintf(cFile,"\n");


	/* 
	   Set the default mode to the first declared mode.
	*/
	mode = root->Node.Program.mode;
	fprintf(cFile,"%s",
		"uint16 curr_mode;\n\n");

	fprintf(hFile,"%s",
		"extern uint16 curr_mode;\n\n");

	/* 
	   Create the array of pointers to the list of
	   task control block in each mode.
	*/
	mode = root->Node.Program.mode;

	fprintf(hFile, "extern TCB_t *TCBList;\n");
	fprintf(cFile, "TCB_t *TCBList;\n");


	fprintf(hFile, "extern TCB_t *TCBList_arr[];\n");
	fprintf(cFile, "TCB_t *TCBList_arr[] = {");

	while( mode ){
	    fprintf(cFile, " TCBList_%s", mode->Node.Mode.name);
	    mode = mode->Node.Mode.Next;
	    if( mode )
	        fprintf(cFile, ",");
	    else
	        fprintf(cFile, " };\n");
	}
	fprintf(hFile,"\n");
	fprintf(cFile,"\n");

	/* 
	   Create the array of pointers to the list of
	   dispatch structures in each mode.
	*/
	mode = root->Node.Program.mode;

	fprintf(hFile, "extern dispatch_t *displist;\n");
	fprintf(cFile, "dispatch_t *displist;\n");

	fprintf(hFile, "extern dispatch_t *displist_arr[];\n");
	fprintf(cFile, "dispatch_t *displist_arr[] = {");


	while( mode ){
	    fprintf(cFile, " displist_%s", mode->Node.Mode.name);
	    mode = mode->Node.Mode.Next;
	    if( mode )
	        fprintf(cFile, ",");
	    else
	        fprintf(cFile, " };\n");
	}
	fprintf(hFile,"\n");
	fprintf(cFile,"\n");



	/* Create an array that contains the number of task in each mode */
	fprintf(hFile, "extern uint16 no_tasks_arr[];\n");
	fprintf(cFile, "uint16 no_tasks_arr[] = {");
	mode = root->Node.Program.mode;
	while( mode ){
	    /* Count the total numer of tasks, hard and soft */
	    no_tasks = 0;
	    no_tasks = countTasks(mode->Node.Mode.tasks->Node.Tasks.hard_tasks);
	    no_tasks += countTasks(mode->Node.Mode.tasks->Node.Tasks.soft_tasks);

	    fprintf(cFile, "%d", no_tasks);
	    if( mode->Node.Mode.Next )
	        fprintf(cFile,", ");
	    mode = mode->Node.Mode.Next;
	}
	no_tasks = 0;
	fprintf(hFile,"\n");
	fprintf(cFile,"};\n\n");



	/* Create an array that contains the timer value for each mode */
	fprintf(hFile, "extern uint16 timerVal_arr[];\n");
	fprintf(cFile, "uint16 timerVal_arr[] = {");
	mode = root->Node.Program.mode;
	while( mode ){
	    fprintf(cFile, "%d", mode->Node.Mode.clock/2);
	    if( mode->Node.Mode.Next )
	        fprintf(cFile,", ");
	    mode = mode->Node.Mode.Next;
	}
	fprintf(hFile,"\n");
	fprintf(cFile,"};\n\n");



	/* 
	   Create code for stack space.
	*/
	mode = root->Node.Program.mode;
	stackSize = 0;
	createStack(mode);


	if( signals_flag ){

	  /* Create an array that contains the number of signals in each mode */
	  fprintf(cFile,"uint16 no_signals;\n");
	  fprintf(hFile,"extern uint16 no_signals;\n");

	  fprintf(hFile, "extern uint16 no_sig_arr[];\n");
	  fprintf(cFile, "uint16 no_sig_arr[] = {");
	  mode = root->Node.Program.mode;
	  while( mode ){
	    /* Count the number of signals in each mode */
	    no_signals = 0;
	    no_signals = countSignals( mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.signals );


	    fprintf(cFile, "%d", no_signals);
	    mode = mode->Node.Mode.Next;
	    if( mode )
	      fprintf(cFile,", ");
	  }
	  no_signals = 0;
	  fprintf(hFile,"\n");
	  fprintf(cFile,"};\n\n");



	  /* 
	     Create the array of pointers to the list of
	     signals in each mode.
	  */
	  mode = root->Node.Program.mode;

	  fprintf(hFile, "extern signal_t *signals;\n");
	  fprintf(cFile, "signal_t *signals;\n");


	  fprintf(hFile, "extern signal_t *signals_arr[];\n");
	  fprintf(cFile, "signal_t *signals_arr[] = {");

	  while( mode ){

	    if( have_signals( mode ) )
	      fprintf(cFile, " signals_%s", mode->Node.Mode.name);
	    else
	      fprintf(cFile, " NULL");

	    mode = mode->Node.Mode.Next;
	    if( mode )
	        fprintf(cFile, ",");
	    else
	        fprintf(cFile, " };\n");
	  }
	  fprintf(hFile,"\n");
	  fprintf(cFile,"\n");


	}


	if( sem_flag ){

	  /* Create an array that contains the number of semaphores in each mode */
	  fprintf(cFile,"uint16 no_semaphores;\n");
	  fprintf(hFile,"extern uint16 no_semaphores;\n");

	  fprintf(hFile, "extern uint16 no_sem_arr[];\n");
	  fprintf(cFile, "uint16 no_sem_arr[] = {");
	  mode = root->Node.Program.mode;
	  while( mode ){
	    /* Count the number of semaphores in each mode */
	    no_semaphores = 0;
	    no_semaphores = countSemaphores( mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores );


	    fprintf(cFile, "%d", no_semaphores);
	    mode = mode->Node.Mode.Next;

	    if( mode )
	      fprintf(cFile,", ");
	  }
	  no_semaphores = 0;
	  fprintf(hFile,"\n");
	  fprintf(cFile,"};\n\n");



	  /* 
	     Create the array of pointers to the list of
	     semaphores in each mode.
	  */
	  mode = root->Node.Program.mode;

	  fprintf(hFile, "extern semaphore_t *semaphore;\n");
	  fprintf(cFile, "semaphore_t *semaphore;\n");


	  fprintf(hFile, "extern semaphore_t *semaphore_arr[];\n");
	  fprintf(cFile, "semaphore_t *semaphore_arr[] = {");

	  while( mode ){

	    if( have_semaphores( mode ) )
	      fprintf(cFile, " semaphore_%s", mode->Node.Mode.name);
	    else
	      fprintf(cFile, " NULL");

	    mode = mode->Node.Mode.Next;
	    if( mode )
	        fprintf(cFile, ",");
	    else
	        fprintf(cFile, " };\n");
	  }
	  fprintf(hFile,"\n");
	  fprintf(cFile,"\n");


	}


	if( wf_flag ){

	  /* Create an array that contains the number of semaphores in each mode */
	  fprintf(cFile,"uint16 no_waitfree;\n");
	  fprintf(hFile,"extern uint16 no_waitfree;\n");

	  fprintf(hFile, "extern uint16 no_wf_arr[];\n");
	  fprintf(cFile, "uint16 no_wf_arr[] = {");
	  mode = root->Node.Program.mode;
	  while( mode ){
	    /* Count the number of semaphores in each mode */
	    no_waitfree = 0;
	    no_waitfree = countWaitfree( mode->Node.Mode.resources->Node.Resources.communications->Node.Communications.wait_free );


	    fprintf(cFile, "%d", no_waitfree);
	    mode = mode->Node.Mode.Next;

	    if( mode )
	      fprintf(cFile,", ");
	  }
	  no_waitfree = 0;
	  fprintf(hFile,"\n");
	  fprintf(cFile,"};\n\n");



	  /* 
	     Create the array of pointers to the list of
	     semaphores in each mode.
	  */
	  mode = root->Node.Program.mode;

	  fprintf(hFile, "extern buffer_t *buffer;\n");
	  fprintf(cFile, "buffer_t *buffer;\n");


	  fprintf(hFile, "extern buffer_t *buffer_arr[];\n");
	  fprintf(cFile, "buffer_t *buffer_arr[] = {");

	  while( mode ){

	    if( have_waitfree( mode ) )
	      fprintf(cFile, " buffer_%s", mode->Node.Mode.name);
	    else
	      fprintf(cFile, " NULL");

	    mode = mode->Node.Mode.Next;
	    if( mode )
	        fprintf(cFile, ",");
	    else
	        fprintf(cFile, " };\n");
	  }
	  fprintf(hFile,"\n");
	  fprintf(cFile,"\n");


	}





	fprintf(hFile, "%s%s%s",
		"/************************************************************\n",
		"   End mode setup\n",
		"*************************************************************/\n\n");

	fprintf(cFile, "%s%s%s",
		"/************************************************************\n",
		"   End mode setup\n",
		"*************************************************************/\n\n");

}



/****************************************************
   Name:      createModes()

   Input:     An Obelix syntax tree with the Mode node as root.

   Output :   -

   Discr:     Analyse the parser tree for tasks 
              and resources in each mode.

   Calls:     -
*****************************************************/
void createModes(tTree modes )
{
    tTree mode = modes;

        while( mode ){
	    createMode(mode);
	    mode = mode->Node.Mode.Next;
	}

}



/****************************************************
   Name:      createMode()

   Input:     The mode to be analysed.

   Output :   -

   Discr:     Genrate code for a mode.

   Calls:
              createFunctionProtos(),
	      createStack(),
	      createTCBList(),
	      createDispatchList(),
	      createResolution(),
	      createSynch(),
	      createWaitFree(),
	      createReaduQueue(),
	      defineInterrupts()
*****************************************************/
void createMode(tTree mode)
{
  tTree semaphores = mode->Node.Mode.resources->Node.Resources.synchronizations
                     ->Node.Synchronizations.semaphores;


  sprintf(mode_name,"%s",mode->Node.Mode.name);

  fprintf(hFile, "%s%s%s\n%s%s%s%s",
	  "/************************************************************\n",
	  "   Mode ",mode_name,
	  "   Set up each individual mode and references needed to\n",
	  "   access each mode.\n",
	  "*************************************************************/",
	  "\n\n");

  fprintf(cFile, "%s%s%s\n%s%s%s%s",
	  "/************************************************************\n",
	  "   Mode ",mode_name,
	  "   Set up each individual mode and references needed to\n",
	  "   access each mode.\n",
	  "*************************************************************/",
	  "\n\n");


  /* Count the total numer of tasks, hard and soft */
  no_tasks = 0;
  no_tasks = countTasks(mode->Node.Mode.tasks->Node.Tasks.hard_tasks);
  no_tasks += countTasks(mode->Node.Mode.tasks->Node.Tasks.soft_tasks);




  createFunctionProtos(mode->Node.Mode.tasks);


  /*
  ***   createArguments(mode->Node.Mode.tasks);
  */


  createResolution(mode->Node.Mode.clock);

  createSynch(mode);
  createWaitFree(mode);
  createSemaphoreListinTask( semaphores, mode->Node.Mode.tasks );

  /*----------------------------------------------------------*/
  /* Flags are set when traversing the syntax tree            */
  /* therefore TCBList and dispatchlist must be built last    */
  /*----------------------------------------------------------*/
  createTCBList(mode->Node.Mode.tasks);

  createDispatchList(mode->Node.Mode.tasks,semaphores);

  /*----------------------------------------------------------*/
  /* Generate the initialization part of the FPS-Calc file    */
  /*----------------------------------------------------------*/
  FPS_Calc_initialise ( mode->Node.Mode.tasks );
  create_BOUND_T_files ( mode->Node.Mode.tasks );
  defineInterrupts();

  fprintf(hFile, "%s%s%s%s%s",
	  "/************************************************************\n",
	  "\n   End mode ",mode_name,
	  "\n*************************************************************/",
          "\n\n");

  fprintf(cFile, "%s%s%s%s%s",
	  "/************************************************************\n",
	  "\n   End mode ",mode_name,
	  "\n*************************************************************/",
          "\n\n");


}



/****************************************************
   Name:      completeFiles()
   Input:     -
   Output :   -
   Discr:     Writes the end of file...
   Calls:     -
*****************************************************/
void completeFiles(){


  fprintf(fpscalcFile, "%s%s%s%s%s%s%s",
          "\n  formulas {\n",
          "\n    ! Calculate the response-time for each task",
          "\n    R[i] = C[i] + sigma(hp, ceiling((R[i])/T[j]) * C[j]);\n",
          "\n    ! Calculate the utilization in the system",
          "\n    ! U must be indexed since lside and rside must be the same.",
          "\n    U[i] = sigma(all, C[j]/T[j]);",
          "\n  }");

  fprintf(fpscalcFile, "%s%s%s%s%s",
      "\n}",
      "\n!/************************************************************",
      "\n!   End of file: ", fpscalcFileName,
      "\n!*************************************************************/\n\n");


  fprintf(incFile, "%s%s%s%s%s",
      "#endif  /* __TASKS_INC_H__ */\n\n",
      "/************************************************************",
      "\n   End of file: ", incFileName,
      "\n*************************************************************/\n\n");

  fprintf(hFile, "%s%s%s%s%s",
      "#endif  /* __TASKS_H__ */\n\n",
      "/************************************************************",
      "\n   End of file: ", hFileName,
      "\n*************************************************************/\n\n");

  fprintf(cFile, "%s%s%s%s",
      "/************************************************************",
      "\n   End of file: ", cFileName,
      "\n*************************************************************/\n\n");
}

void create_BOUND_T_files ( tTree tasks )
{
  tTree soft_task = tasks->Node.Tasks.soft_tasks;
  tTree hard_task = tasks->Node.Tasks.hard_tasks;

  fprintf( boundtFile, "boundt_h8_300.exe -assert timertick_ass.txt -lego -srec=romdump.srec -sym=lego-rom-sym.txt kernel.coff-h8300 ");

  while ( hard_task )
    {
      if (hard_task->Node.Task.period >= 0 )
        fprintf ( boundtFile, "_%s ", hard_task->Node.Task.routine );
      hard_task = hard_task->Node.Task.Next;
    }
  while ( soft_task )
    {
      if (soft_task->Node.Task.period >= 0 )
        fprintf ( boundtFile, "_%s ", soft_task->Node.Task.routine );
      soft_task = soft_task->Node.Task.Next;
    }
  fprintf ( boundtFile, "\n");

}

/****************************************************
   Name:      FPS_Calc_initialise ( )
   Input:     -
   Output :   -
   Discr:     Prints task attribute to the FPS Calc file
   Calls:     -
*****************************************************/
void  FPS_Calc_initialise ( tTree mode )
{

  fprintf( fpscalcFile, "  initialise {");

  fpscalc_initialise_period ( mode ); 
  fpscalc_initialise_wcet ( mode ); 
  fpscalc_initialise_deadline ( mode ); 
  fpscalc_initialise_priority ( mode ); 

  fprintf( fpscalcFile, "  }");
}

void fpscalc_initialise_period ( tTree tasks )
{
    tTree soft_task = tasks->Node.Tasks.soft_tasks;
    tTree hard_task = tasks->Node.Tasks.hard_tasks;

    fprintf ( fpscalcFile, "\n\n! Periods");
    while ( hard_task )
      {
        if (hard_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    T[%s] = %d;", hard_task->Node.Task.name, hard_task->Node.Task.period * resolution );

        hard_task = hard_task->Node.Task.Next;
      }

    while ( soft_task )
      {
        if (soft_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    T[%s] = %d;", soft_task->Node.Task.name, soft_task->Node.Task.period * resolution );

        soft_task = soft_task->Node.Task.Next;
      }
}

void fpscalc_initialise_wcet ( tTree tasks )
{
    tTree soft_task = tasks->Node.Tasks.soft_tasks;
    tTree hard_task = tasks->Node.Tasks.hard_tasks;

    fprintf( fpscalcFile, "\n\n! WCETs");
    while ( hard_task )
      {
        if (hard_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    C[%s] = --;", hard_task->Node.Task.name);

        hard_task = hard_task->Node.Task.Next;
      }

    while ( soft_task )
      {
        if (soft_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    C[%s] = --;", soft_task->Node.Task.name);

        soft_task = soft_task->Node.Task.Next;
      }

}

void fpscalc_initialise_deadline ( tTree tasks )
{
    tTree soft_task = tasks->Node.Tasks.soft_tasks;
    tTree hard_task = tasks->Node.Tasks.hard_tasks;

    fprintf( fpscalcFile, "\n\n! Deadlines");
    while ( hard_task )
      {
        if (hard_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    D[%s] = %d;", hard_task->Node.Task.name, hard_task->Node.Task.deadline * resolution );

        hard_task = hard_task->Node.Task.Next;
      }

    while ( soft_task )
      {
        if (soft_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    D[%s] = %d;", soft_task->Node.Task.name, soft_task->Node.Task.deadline * resolution );

        soft_task = soft_task->Node.Task.Next;
      }
}

void fpscalc_initialise_priority ( tTree tasks )
{
    tTree soft_task = tasks->Node.Tasks.soft_tasks;
    tTree hard_task = tasks->Node.Tasks.hard_tasks;

    fprintf( fpscalcFile, "\n\n! Priority");
    while ( hard_task )
      {
        if (hard_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    P[%s] = %d;", hard_task->Node.Task.name,no_tasks - 1 - hard_task->Node.Task.priority);

        hard_task = hard_task->Node.Task.Next;
      }

    while ( soft_task )
      {
        if (soft_task->Node.Task.period >= 0 )
          fprintf ( fpscalcFile, "\n    P[%s] = %d;", soft_task->Node.Task.name,no_tasks - 1 -  soft_task->Node.Task.priority);

        soft_task = soft_task->Node.Task.Next;
      }
}


/****************************************************
   Name:      defineInterrupts()
   Input:     -
   Output :   -
   Discr:     Defines interupts.
   Calls:     -
*****************************************************/
void defineInterrupts(void){
  fprintf(hFile, "%s%s%s",
      "/************************************************************",
      "\n   Interrupts\n",
      "*************************************************************/\n\n");

  fprintf(hFile, "%s%s",
      "#define __INTERRUPTS__\n",
      "#define __IRQ__\n\n");

  fprintf(hFile,"%s%s%s",
      "/************************************************************",
      "\n   End Interrupts\n",
      "*************************************************************/\n\n");

  fprintf(incFile, "%s%s%s",
      "/************************************************************",
      "\n   Interrupts\n",
      "*************************************************************/\n\n");

  fprintf(incFile, "%s%s",
      "#define __INTERRUPTS__\n",
      "#define __IRQ__\n\n");

  fprintf(incFile,"%s%s%s",
      "/************************************************************",
      "\n   End Interrupts\n",
      "*************************************************************/\n\n");

}




/****************************************************
   Name:      createWaitFree()
   Input:     tTree mode, a mode branch of the syntax tree
   Output :   -
   Discr:     Generates code for the waitfree IPC
   Calls:     createTaskBufCtrl()
*****************************************************/
void createWaitFree(tTree mode){
  tTree wait_free = mode->Node.Mode.resources->Node.Resources.communications->Node.Communications.wait_free;
  tTree tasks = mode->Node.Mode.tasks;
  tTree temp = wait_free;
  int no_waitfree = 0, i = 0, j = 0;
  char buf_id[256];


  /* Comment header */
  if(wait_free){
    wf_flag = 1;
    no_waitfree = countWaitfree( wait_free );

    fprintf(incFile,"%s%s%s",
        "/************************************************************",
        "\n   Wait free IPC\n",
        "*************************************************************/\n\n");
    fprintf(hFile,"%s%s%s",
        "/************************************************************",
        "\n   Wait free IPC\n",
        "*************************************************************/\n\n");
    fprintf(cFile,"%s%s%s",
        "/************************************************************",
        "\n   Wait free IPC\n",
        "*************************************************************/\n\n");

    temp = wait_free;
    /* for each waitfree there has to be some variables */
    for(i = 0; i < no_waitfree; i++){
      /* first write the user defined type variable */
      fprintf(cFile, "int16%s%d%s%s%s%s)0"," wfbuf", i, "buf_",mode_name,"[] = {(",temp->Node.WaitFree.type);
      for(j = 1; j < temp->Node.WaitFree.buffers; j++)
        fprintf(cFile, ",(%s)%s",temp->Node.WaitFree.type,"0");

      fprintf(cFile, "%s", "};\n");

      /* now the uint16 variable */
      fprintf(cFile, "%s%s%d%s%s%s",
          CLOCKTYPE, " wfbuf", i, "bufsem_",mode_name,"[] = {0");
      for(j = 1; j < temp->Node.WaitFree.buffers; j++)
        fprintf(cFile, "%s", ",0");
      fprintf(cFile, "%s", "};\n");

      /* here goes the llnode_t */
      fprintf(cFile, "%s%d%s%s%s%d%s%s%s","llnode_t wfbuf", i, "oldest_",mode_name,"[] = {{0,&wfbuf",i,"oldest_",mode_name,"[1]}");
      for(j = 1; j < temp->Node.WaitFree.buffers - 1; j++)
    fprintf(cFile, "%s%d%s%d%s%s%s%d%s",
        ",{", j, ",&wfbuf", i, "oldest_",mode_name,"[", j+1, "]}");
      fprintf(cFile, "%s%d%s",
          ",{", j, ",0}};\n");

      /* the variable to match waitfree id */
      fprintf(cFile, "%s %s%s%d%s",
         CLOCKTYPE, temp->Node.WaitFree.name, " = ", i, ";\n\n");

      //if(i)
      //fprintf(cFile, ",");

      /* next waitfree in the tree */
      temp = temp->Node.WaitFree.Next;
    }

    temp = wait_free;

    /* Now the array of waitfree channels has to be declared and
       initiated, the number of waitfrees will first be declared */
    fprintf(hFile, "%s%s",
        "#define __WAITFREE__\n",
        "#include <os_waitfree.h>\n");
    fprintf(incFile,"#define __WAITFREE__\n");

    for(i=0;i<no_waitfree;i++,temp=temp->Node.WaitFree.Next)
    {
      fprintf(hFile, "extern %s %s;\n\n",
         CLOCKTYPE, temp->Node.WaitFree.name);
    }


    temp = wait_free;

    /* The first instance of the array */
    fprintf(cFile, "%s%s%s%d%s%s%s%s%s%s%s%s%s%s%d%s",
        "buffer_t buffer_",mode_name,"[", no_waitfree, "] = {\n",
        "{&wfbuf0buf_",mode_name,"[0], &wfbuf0bufsem_",mode_name,"[0], &wfbuf0oldest_",mode_name,"[0], &wfbuf0oldest_",mode_name,"[",
        temp->Node.WaitFree.buffers - 1,
        "]}");

    temp = temp->Node.WaitFree.Next;

    /* loop for each waitfree */
    for(i = 1; i < no_waitfree; i++){
      sprintf(buf_id, "%s%d", "&wfbuf", i);
      fprintf(cFile, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s",
          ",\n{", buf_id, "buf_",mode_name,"[0], ", buf_id, "bufsem_",mode_name,"[0], ",
          buf_id, "oldest_",mode_name,"[0], ", buf_id, "oldest_",mode_name,"[",
          temp->Node.WaitFree.buffers - 1, "]}\n");

      temp = temp->Node.WaitFree.Next;
    }

    fprintf(cFile, "%s", "};\n\n");

    /* Create code for each task buffer ctrl */
    createTaskBufCtrl(wait_free, tasks->Node.Tasks.soft_tasks);
    createTaskBufCtrl(wait_free, tasks->Node.Tasks.hard_tasks);

    fprintf(incFile,"%s%s%s",
        "/************************************************************",
        "\n   End Wait free\n",
        "*************************************************************/\n\n");
    fprintf(hFile,"%s%s%s",
        "/************************************************************",
        "\n   End Wait free\n",
        "*************************************************************/\n\n");
    fprintf(cFile,"%s%s%s",
        "\n/************************************************************",
        "\n   End Wait free\n",
        "*************************************************************/\n\n");
  }


}


/****************************************************
   Name:      createTaskBufCtrl()
   Input:     tTree wait_free, a waitfree branch,
              tTree tasks, a task branch of the ST.
   Output :   -
   Discr:     Generates waitfree task buff control code.
   Calls:     checkWf(),
              isWriter()
*****************************************************/
void createTaskBufCtrl(tTree wait_free, tTree tasks){
  tTree temp_task = tasks;
  tTree temp_wf = wait_free;
  int temp_index = 0, no_wf = 0, i = 0;
  wfel *list = NULL, *temp_el = NULL;
  char w_or_r[20];

  /* Go for each task */
  while(temp_task){
    no_wf = 0;
    temp_index = 0;
    temp_wf = wait_free;
    /* Go through all waitfree channels for each task */
    while(temp_wf){
      /* Is current task user of current wf channel? */
      if(checkWf(temp_task, temp_wf)){
    /* list the wf that current task is using */
    no_wf++;
    temp_el = getWfEl(temp_index, isWriter(temp_task, temp_wf));
    temp_el->next = list;
    list = temp_el;
      }
      temp_index++;
      temp_wf = temp_wf->Node.WaitFree.Next;
    }

    /* Write out the buff control data, the first row only
       describes the number of wf used. */
    fprintf(cFile, "%s%s%s%s%s%d%s",
        "bufctrl_t ",
        temp_task->Node.Task.name,
        "_bufctrl_",mode_name,"[] = {\n\t{",
        no_wf,
        ",0,FALSE,0}");

    temp_el = list;
    /* loop for the rest of wf's */
    for(i = 0; i < no_wf; i++){
      if(temp_el->is_writer)
    strcpy(w_or_r, "WF_WRITER,TRUE");
      else
    strcpy(w_or_r, "WF_READER,FALSE");
      fprintf(cFile, "%s%d%c%s%s",
          ",\n\t{", temp_el->index, ',',
          w_or_r, ",0}");
      temp_el = temp_el->next;
    }

    fprintf(cFile, "%s", "};\n");

    /* Let go of current list */
    freeWfElList(&list);
    /* and go for the next task */
    temp_task = temp_task->Node.Task.Next;
  }
}


/****************************************************
   Name:      createSynch()
   Input:     tTree mode, a mode branch of the syntax tree
   Output :   -
   Discr:     Generates the code for semaphores and signals,
              also builds up a representation of the task list
          and generates the code for the task list.
   Calls:     buildTaskList()
              createSemaphores()
          createSignals()
          createTaskList()
*****************************************************/
void createSynch(tTree mode){
  tTree semaphores = mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores;
  tTree signals = mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.signals;
  tTree *task_list = NULL;

  task_list = (tTree*)malloc( sizeof(tTree) * no_tasks );

  /* build up a representation of the task list */
  buildTaskList(mode, task_list);


/*  Debug printout of task list */
  /*
  {
   int i;
    printf("************* task_list **************\n");

    for(i = 0; i < no_tasks; i++)
      {
	printNode(task_list[i]);
      }
  }
  */


  createSemaphores(semaphores, task_list);
  createSignals(signals, task_list);

  free(task_list);
}


/****************************************************
   Name:      createSignals()
   Input:     tTree in_sig, signals in the mode
              tTree task_list[], representation of the
              task list
   Output :   -
   Discr:     Generates the signal code.
   Calls:     createSigUserArray(),
              createSigBlockedArray(),
          countUsers()
*****************************************************/
void createSignals(tTree in_sig, tTree task_list[]){
  tTree signals = in_sig;
  int no_signals = 0, no_users = 0, i;
  int no_blocked_tasks[500]; /* maximum no of tasks in the mode, no_tasks */


  /* Comment header */
  if( signals ){

    signals_flag = 1;
    no_signals = countSignals( signals );


    fprintf(incFile,"%s%s%s",
        "/************************************************************",
        "\n   Signals\n",
        "*************************************************************/\n\n");
    fprintf(hFile,"%s%s%s",
        "/************************************************************",
        "\n   Signals\n",
        "*************************************************************/\n\n");
    fprintf(cFile,"%s%s%s",
        "/************************************************************",
        "\n   Signals\n",
        "*************************************************************/\n\n");
    fprintf(hFile, "%s%s%s%s%s%d%s",
        "#define __SIGNALS__\n",
        "#include <os_signals.h>\n",
        "\nextern signal_t signals_",mode_name,"[", no_signals, "];\n");
    fprintf(incFile,"#define __SIGNALS__\n");


    /* create the array of users and the array of blocked tasks for each signal */
    signals = in_sig;
    for(i = 0; i < no_signals; i++){
      createSigUserArray(signals, i, task_list);

      no_blocked_tasks[i] = createSigBlockedArray(signals, i, task_list);
      fprintf(cFile, "%s %s%s%d%s",
          "sigid_t",
          signals->Node.Signal.name,
          " = ", i, ";\n");
      fprintf(hFile, "%s%s %s%s",
          "extern ",
          "sigid_t",
          signals->Node.Signal.name,
          ";\n");
      signals = signals->Node.Signal.Next;
    }

    fprintf(hFile, "\n");

    /* Now create the signals, an array of signal_t */
    signals = in_sig;
    fprintf(cFile, "%s%s%s%d%s",
        "signal_t signals_",mode_name,"[",
        no_signals,
        "] = {");

    /* Initiate each signal */
    for(i = 0; i < no_signals; i++){
      no_users = countUsers(signals->Node.Signal.user);
      if(i)
    fprintf(cFile, "%c", ',');
      fprintf(cFile, "%s%s%s%d%s%c%d%c%s%s%d%s%c%d%c",
          "{",mode_name,"_sig", i, "_1", ',', no_users, ',',
          mode_name,"_sig", i, "_2",
          ',', no_blocked_tasks[i],
          '}');
      signals = signals->Node.Signal.Next;
    }

    fprintf(cFile, "%s", "};\n\n");

    /* End of signals */
    fprintf(incFile, "%s%s%s",
        "/************************************************************",
        "\n   End Signals\n",
        "*************************************************************/\n\n");
    fprintf(hFile, "%s%s%s",
        "/************************************************************",
        "\n   End Signals\n",
        "*************************************************************/\n\n");
    fprintf(cFile, "%s%s%s",
        "/************************************************************",
        "\n   End Signals\n",
        "*************************************************************/\n\n");
  }
}

/****************************************************
   Name:      createSigBlockedArray()
   Input:     tTree signals, the signal to work on,
              int no, signal number,
              tTree task_list[], representation of the
              task list
   Output :   number of tasks that is blocked by the
              signal.
   Discr:     Generates the blocked array for the signal
              and returns the number of blocked tasks.
   Calls:     getUserTaskListIndex(),
              countUsers()
*****************************************************/
int createSigBlockedArray(tTree signals, int no, tTree task_list[]){
  tTree user = signals->Node.Signal.user;
  tTree temp_user = NULL;
  int i = 0, index = 0, no_blocked = 0;
  int no_users = 0;

  /* create the the array of users triggered by the signal */
  fprintf(cFile, "%s%s%s%d%s",
	  "taskid_t ",mode_name,
	  "_sig", no, "_2[] = {");

  /* Fill in all tasks that, initially, is blocked by the signal */
  no_users = countUsers(user);
  while(i < no_users){
    if(user){
      index = getUserTaskListIndex(user,
                   task_list,
                   no_tasks);
      temp_user = task_list[index];
      if(temp_user->Node.Task.activator != NULL){
          if(temp_user->Node.Task.activator->Kind == kStringConst){
            if(!strcmp(temp_user->Node.Task.activator->Node.StringConst.Value,
                       signals->Node.Signal.name)){
                if(i){
                    fprintf(cFile, "%c", ',');
                }
                fprintf(cFile, "%d",
                        getUserTaskListIndex(user,
                     task_list,
                     no_tasks));
                no_blocked++;
                i++;
                }
            }
        }
        user = user->Node.User.Next;
    }
    else{
      if(i)
        fprintf(cFile, "%c", ',');
      fprintf(cFile, "%c", '0');
      i++;
    }
  }

  fprintf(cFile, "%s", "};\n");
  return no_blocked;
}

/****************************************************
   Name:      createSigUserArray()
   Input:     tTree signals, the signal to work on,
              int no, signal number,
              tTree task_list[], representation of the
              task list
   Output :   -
   Discr:     Generates the array of users of the signal.
   Calls:     getUserTaskListIndex(),
              countUsers()
*****************************************************/
void createSigUserArray(tTree signals, int no, tTree task_list[]){
  tTree user = signals->Node.Signal.user;

  /* Create the array of users, named by their index in the task list */
  fprintf(cFile, "%s%s%s%d%s%d",
	  "taskid_t ",mode_name,
	  "_sig", no, "_1[] = {", getUserTaskListIndex(user,task_list,no_tasks));
  user = user->Node.User.Next;
  while(user){
    fprintf(cFile, "%c%d", ',', getUserTaskListIndex(user,
                             task_list,
                             no_tasks));
    user = user->Node.User.Next;
  }
  fprintf(cFile, "%s", "};\n");
}


/****************************************************
   Name:      createSemaphores()
   Input:     tTree in_sem, the semphores of a mode branch
              in the syntax tree,
              tTree task_list[], representation of the
          task list
~   Output :   -
   Discr:     Generates the semaphore code.
   Calls:     createSemUserArray(),
          countUsers(),
          getSemaphoreTaskListIndex()
*****************************************************/
void createSemaphores(tTree in_sem, tTree task_list[]){
    tTree semaphores = in_sem;
    int i, no_user = 0;


        //printf("Enter createSemphores\n");


        if( semaphores ){
            sem_flag=1;
	    no_semaphores = countSemaphores( semaphores );

            fprintf(incFile,"%s%s%s",
                "/************************************************************",
                "\n   Semaphores\n",
                "*************************************************************/\n\n");
            fprintf(incFile,"#define __SEMAPHORE__\n");

            fprintf(hFile,"%s%s%s",
                "/************************************************************",
                "\n   Semaphores\n",
                "*************************************************************/\n\n");
            fprintf(hFile, "%s%s",
                "#define __SEMAPHORE__\n",
                "#include <os_semaphore.h>\n");


            fprintf(cFile,"%s%s%s",
                "/************************************************************",
                "\n   Semaphores\n",
                "*************************************************************/\n\n");



            /* create the array of users for each semaphore */
            for(i = 0; i < no_semaphores; i++){
                createSemUserArray(semaphores, i, task_list);
                semaphores = semaphores->Node.Semaphore.Next;
            }


            /* Now create the semaphores, an array of semaphore_t */
            semaphores = in_sem;
            fprintf(cFile, "%s%s%s%d%s",
                "semaphore_t semaphore_",mode_name,"[",
                no_semaphores,
                "] = {");

            /* For each semaphore */
            for(i=0; i < no_semaphores;i++){
                no_user = countUsers(semaphores->Node.Semaphore.user);

                if(i)
                    fprintf(cFile, "%c", ',');


                /*-------------------------------------------------*/
                /* The priority-ceiling of a semaphore is equal to */
                /* the highest priority (of the tasks that have    */
                /* access to the semaphore) +1.                    */
                /* The priority and ceiling are set in             */
                /* semact.c:make_unique_priority()                 */
                /*-------------------------------------------------*/
                fprintf(cFile, "%s%d%c%s%c%d%c%d%c",
                    "{sem", i,'_', mode_name, ',', no_user, ',',
                    semaphores->Node.Semaphore.ceil,
                    '}');
                    semaphores = semaphores->Node.Semaphore.Next;
            }

            fprintf(cFile, "%s", "};\n\n");
            /* End of semaphores */




            fprintf(hFile, "%s%s%s",
                "\n/************************************************************",
                "\n   End Semaphores\n",
                "*************************************************************/\n\n");
            fprintf(cFile, "%s%s%s",
                "/************************************************************",
                "\n   End Semaphpores\n",
                "*************************************************************/\n\n");
        }


        //printf("Leaving createSemaphores\n");
}


/****************************************************
   Name:      createSemUserArray()
   Input:     tTree semaphores, a semaphore branch of the syntax tree,
              int no, semaphore enumeration,
              tTree task_list[], the representation of the task list.
   Output:    -
   Discr:     Creates the array of users for semaphore, no is used
              when nameing the array.
   Calls:     getUserTaskListIndex()
*****************************************************/
void createSemUserArray(tTree semaphores, int no, tTree task_list[]){
  tTree user = semaphores->Node.Semaphore.user;

  /* Create the array of users, named by their index in the task list */

  fprintf(cFile, "%s%s%d%c%s%s%d",
      "taskid_t ",
      "sem", no,'_',mode_name, "[] = {", getUserTaskListIndex(user,
                            task_list,
                            no_tasks));
  user = user->Node.User.Next;
  while(user){
    fprintf(cFile, "%c%d", ',', getUserTaskListIndex(user,
                             task_list,
                             no_tasks));
    user = user->Node.User.Next;
  }

  /* Declare semaphore user name */
  fprintf(cFile, "%s", "};\n");


  fprintf(cFile, "%s %s%s%d%s",
      "semid_t",
      semaphores->Node.Semaphore.name,
      " = ", no, ";\n");

   fprintf(hFile, "%s%s%s",
      "extern semid_t ",
      semaphores->Node.Semaphore.name,
       ";\n");
}



void  createSemaphoreListinTask(tTree semaphores, tTree tasks)
{

    tTree soft_task                     = tasks->Node.Tasks.soft_tasks;
    tTree hard_task                     = tasks->Node.Tasks.hard_tasks;
    tTree tmpsemaphores                 = NULL;
    tTree user                          = NULL;
    unsigned int *semaphoreidx_in_task  = NULL;
    unsigned int nrofsemaphores_taken   = 0;
    unsigned int semaphore_idx          = 0;
    unsigned int semaphore_taken        = 0;


        if( semaphores ){

            semaphoreidx_in_task = malloc( sizeof(unsigned int) * no_semaphores );

            fprintf(hFile,"%s%s%s",
                "/************************************************************",
                "\n   List of semaphores taken for each task \n",
                "*************************************************************/\n\n");


            fprintf(cFile,"%s%s%s",
                "/************************************************************",
                "\n   List of semaphores taken for each task \n",
                "*************************************************************/\n\n");


            while( soft_task ){
                semaphore_idx           = 0;
                nrofsemaphores_taken    = 0;
                semaphore_taken         = 0;
                tmpsemaphores           = semaphores;

                while( tmpsemaphores ){
                    user = tmpsemaphores->Node.Semaphore.user;
                    while( user ){
                        if( !strcmp(user->Node.User.string,soft_task->Node.Task.name)  ){
                            semaphoreidx_in_task[nrofsemaphores_taken] = (semaphore_idx << 1);
                            nrofsemaphores_taken++;
                            semaphore_taken=1;
                        }
                        user = user->Node.User.Next;
                    }
                    tmpsemaphores = tmpsemaphores->Node.Semaphore.Next;
                    semaphore_idx++;
                }
                if( semaphore_taken ){
                    fprintf(cFile,"%s task_%s_semaphoretaken_%s[%d]={%d",
                        SEMAPHORELIST_T,soft_task->Node.Task.name,mode_name ,nrofsemaphores_taken+1,nrofsemaphores_taken);

                    fprintf(hFile,"extern %s task_%s_semaphoretaken_%s[%d];\n",
                        SEMAPHORELIST_T,soft_task->Node.Task.name,mode_name ,nrofsemaphores_taken+1);

                    for( semaphore_idx = 0 ; semaphore_idx < nrofsemaphores_taken ; semaphore_idx++){
                        fprintf(cFile,",%d",semaphoreidx_in_task[semaphore_idx]);
                    }
                    fprintf(cFile,"%s","};\n");
                    soft_task->Node.Task.semaphore_flag = 1;
                } else
                    soft_task->Node.Task.semaphore_flag = 0;

                soft_task = soft_task->Node.Task.Next;
            }


            while( hard_task ){
                semaphore_idx           = 0;
                nrofsemaphores_taken    = 0;
                semaphore_taken         = 0;
                tmpsemaphores           = semaphores;

                while( tmpsemaphores ){
                    user = tmpsemaphores->Node.Semaphore.user;
                    while( user ){
                        if( !strcmp(user->Node.User.string,hard_task->Node.Task.name)  ){
                            semaphoreidx_in_task[nrofsemaphores_taken] = (semaphore_idx << 1);
                            nrofsemaphores_taken++;
                            semaphore_taken=1;
                        }
                        user = user->Node.User.Next;
                    }
                    tmpsemaphores = tmpsemaphores->Node.Semaphore.Next;
                    semaphore_idx++;
                }

                if( semaphore_taken ){
                    fprintf(cFile,"%s task_%s_semaphoretaken_%s[%d]={%d",
                        SEMAPHORELIST_T,hard_task->Node.Task.name,mode_name,nrofsemaphores_taken+1,nrofsemaphores_taken);

                    fprintf(hFile,"extern %s task_%s_semaphoretaken_%s[%d];\n",
                        SEMAPHORELIST_T,hard_task->Node.Task.name,mode_name,nrofsemaphores_taken+1);

                    for( semaphore_idx = 0 ; semaphore_idx < nrofsemaphores_taken ; semaphore_idx++){
                        fprintf(cFile,",%d",semaphoreidx_in_task[semaphore_idx]);
                    }
                    fprintf(cFile,"%s","};\n");
                    hard_task->Node.Task.semaphore_flag = 1;
                } else
                    hard_task->Node.Task.semaphore_flag = 0;


                hard_task = hard_task->Node.Task.Next;
            }


            fprintf(incFile,"%s%s%s",
                "\n/************************************************************",
                "\n   End List of semaphores taken for each task \n",
                "*************************************************************/\n\n");


            fprintf(hFile,"%s%s%s",
                "\n/************************************************************",
                "\n   End List of semaphores taken for each task \n",
                "*************************************************************/\n\n");


            fprintf(cFile,"%s%s%s",
                "\n/************************************************************",
                "\n   End List of semaphores taken for each task \n",
                "*************************************************************/\n\n");


            free(semaphoreidx_in_task);
        }
}




/****************************************************
   Name:      buildTaskList()
   Input:     tTree mode, a mode branch of the syntax tree
              tTree task_list[], the representation of the task list.
   Output:    -
   Discr:     Builds up a representation of the task list,
              the task list consists of tasks (tcb) and semaphores
          ordered by priority and ceiling, lowest first.
   Calls:     -
*****************************************************/
void buildTaskList(tTree mode, tTree task_list[]){
    tTree soft_tasks = mode->Node.Mode.tasks->Node.Tasks.soft_tasks;
    tTree hard_tasks = mode->Node.Mode.tasks->Node.Tasks.hard_tasks;
    int i = 0;


        while( soft_tasks ){
            task_list[i] = soft_tasks;
            soft_tasks = soft_tasks->Node.Task.Next;
            i++;
        }

        start_hardtasks = i;

        while( hard_tasks ){
            task_list[i] = hard_tasks;
            hard_tasks = hard_tasks->Node.Task.Next;
            i++;
        }

}



/****************************************************
   Name:      createResolution()
   Input:     int res, user assigned resolution i us.
   Output :   -
   Discr:     Defines the clock resolution.
   Calls:     -
*****************************************************/
void createResolution(int res){
  fprintf(hFile, "%s%s%s",
      "/************************************************************",
      "\n   Clock resolution\n",
      "*************************************************************/\n\n");
  fprintf(cFile, "%s%s%s",
      "/************************************************************",
      "\n   Clock resolution\n",
      "*************************************************************/\n\n");
  fprintf(hFile, "%s%s%s%s%s",
      "extern ",
      CLOCKTYPE,
      " timerVal_",mode_name,";\n\n");
  fprintf(cFile, "%s%s%s%s%d%s",
      CLOCKTYPE,
      " timerVal_",mode_name," = ",
      res / 2,
      ";\n\n");

  resolution = res;

}






/****************************************************
   Name:      createDispatchList()
   Input:     tasks, the tasks branch of a mode in the
              syntax tree.
   Output:    -
   Descr:     Generates the code for the dispacth list.
   Calls:     countTasks(),
              createTaskInDispachList()
*****************************************************/
void createDispatchList(tTree tasks, tTree semaphore ){
    int noTasks = 0, is_soft_task = 0;

        /* Count the total numer of tasks, hard and soft */
        noTasks = countTasks(tasks->Node.Tasks.hard_tasks);
        noTasks += countTasks(tasks->Node.Tasks.soft_tasks);

        /* Header of the dispatch list */
        fprintf(hFile, "%s%s%s%s%s",
            "/************************************************************",
            "\n   Dispatch list",
            "\n   STATE, Ticks to deadline, SP, void (*PC)(void), CCR, R0...R6,",
            "\n   Ticks to new period, current priority, original priority\n",
            "*************************************************************/\n\n");

        fprintf(cFile, "%s%s%s%s%s",
            "/************************************************************",
            "\n   Dispatch list",
            "\n   STATE, Ticks to deadline, SP, void (*PC)(void), CCR, R0...R6,",
            "\n   Ticks to new period, current priority, original priority\n",
            "*************************************************************/\n\n");

        fprintf(hFile, "%s%s%s%s%d%s",
            "extern ",
            "dispatch_t displist_",
	    mode_name,
	    "[",
            noTasks,
            "];\n");

        fprintf(cFile, "%s%s%s%d%s",
            "dispatch_t displist_",
	    mode_name,
	    "[",
            noTasks,
            "] = {\n");

        /* Generate code for each task */
        if(tasks->Node.Tasks.soft_tasks)
            is_soft_task = 1;

        createTaskInDispatchList(tasks->Node.Tasks.soft_tasks, semaphore);

        if(is_soft_task && tasks->Node.Tasks.hard_tasks)
            fprintf(cFile, "%s", ",\n");

        createTaskInDispatchList(tasks->Node.Tasks.hard_tasks, semaphore);

        fprintf(hFile, "%c", '\n');
        fprintf(hFile, "%s%s%s",
            "/************************************************************",
            "\n   End Dispatch list\n",
            "*************************************************************/\n\n");

        fprintf(cFile, "%s", "};\n\n");
        fprintf(cFile, "%s%s%s",
            "/************************************************************",
            "\n   End Dispatch list\n",
            "*************************************************************/\n\n");
}


/****************************************************
   Name:      createTaskInDispachtList()
   Input:     task, a task in the syntax tree
   Output:    -
   Discr:     Generates the code that defines tasks
              in the system recursively.
   Calls:     createTaskInDispatchList(), recursive
*****************************************************/
void createTaskInDispatchList(tTree task , tTree semaphore){
    char startAddress[MAX_STRING_LENGTH];
    char errorHandler[MAX_STRING_LENGTH];
    char arguments[MAX_STRING_LENGTH];
    char state[20];
    tTree activator = NULL;

        if(task){
            /* Get strings for startAddress, errorHandler and arguments */
            strcpy(startAddress, task->Node.Task.routine);

            /* Check for the error routine */
            if(task->Node.Task.err_routine)
                strcpy(errorHandler, task->Node.Task.err_routine->Node.StringConst.Value);
            else
                strcpy(errorHandler, DEF_ERROR_ROUTINE);

            /* Check for argument */
            if(task->Node.Task.argument)
                strcpy(arguments, task->Node.Task.argument->Node.StringConst.Value);
            else
                strcpy(arguments, "NULL");

            /* Check activator, state depends on whitch kind of acivator used */
            activator = task->Node.Task.activator;

            /* if the task has a period */
            if(activator == NULL){
                /* put the idle task in state RUNNING */
                if(task->Node.Task.priority == 0)
                    strcpy(state, "RUNNING");
                else
                    strcpy(state, "WAITING");
            } else
                /* not by a period,the task is triggerd by a signal */
                strcpy(state, "SIG_BLOCKED");



            /*----------------------------------------------------*/
            /* Generate the code that defines a task, that is:    */
            /* {STATE, DL, SP, start_address, CCR, R0...R6 */
            /* Ticks to new period,current_priority,              */
            /* original_priority}                                 */
            /*----------------------------------------------------*/
            if(!strcmp(arguments, "NULL")){
                fprintf(cFile, "%c%s%c%d%c%s%s%s%s%d,%d,%d,%c",
                    '{', state, ',', task->Node.Task.deadline,
                    ',', task->Node.Task.routine, ",0,(cpureg_t)", arguments,
                    ",0,0,0,0,0,0,", task->Node.Task.offset,task->Node.Task.priority,task->Node.Task.priority, '}');
            }
            else{
                fprintf(cFile, "%c%s%c%d%c%s%s%s%s%d,%d,%d,%c",
                    '{', state, ',', task->Node.Task.deadline,
                    ',', task->Node.Task.routine, ",0,(cpureg_t)&", arguments,
                    ",0,0,0,0,0,0,", task->Node.Task.offset,task->Node.Task.priority,task->Node.Task.priority, '}');
            }

            if(task->Node.Task.Next)
                fprintf(cFile, "%s", ",\n");

            /* Go for next task in the tree */
            createTaskInDispatchList(task->Node.Task.Next, semaphore);
        }
}



/****************************************************
   Name:      createStack()
   Input:     tasks, the tasks branch of a mode in
              the syntax tree
   Output:    -
   Descr:     Generates the code that declares and
              initiates the user stacks.
   Calls:     calculateStackSpace()
   Comment:   This part of Obelix is very target-
              system dependent.
*****************************************************/
void createStack(tTree mode)
{

  while( mode ){
    old_stackSize = stackSize;
    stackSize=0;

    /* Calculate the stack space */
    calculateStackSpace(mode->Node.Mode.tasks->Node.Tasks.soft_tasks);
    calculateStackSpace(mode->Node.Mode.tasks->Node.Tasks.hard_tasks);

    if( old_stackSize > stackSize )
      stackSize = old_stackSize;

    mode = mode->Node.Mode.Next;
  }


  if(stackSize){
    fprintf(hFile, "extern uint16 stacksize;\n\n");
    fprintf(cFile, "uint16 stacksize = %d;\n\n", stackSize);
    fprintf(hFile, "%s%s%s%d%s",
	    "extern ",
	    STACKTYPE,
	    " stack_mem[",
	    stackSize,
	    "];\n\n");
    fprintf(cFile, "%s%s%d%s",
	    STACKTYPE,
	    " stack_mem[",
	    stackSize,
	    "];\n\n");
  }

}






/****************************************************
   Name:      calculateStackSpace()

   Input:     task, a list of tasks from the syntax tree

   Output:    -

   Descr:     Calculate the needed stack space for the
              tasks in a mode.


   Calls:     calculateStackSpace(), recursive

   Comment:   This part of Obelix is very target-
              system dependent.
*****************************************************/
void calculateStackSpace(tTree task){

  if(task){
    /*    strcpy(startAddress, task->Node.Task.routine); */
    stackSize += task->Node.Task.stack+2;
    calculateStackSpace(task->Node.Task.Next);

  }
}

/****************************************************
   Name:      createFunctionProtos()
   Input:     tasks, the task branch of a mode in the
              syntax tree
   Output:    -
   Descr:     Generates function prototypes for each
              task start and error routine.
   Calls:     createPrototype()
*****************************************************/
void createFunctionProtos(tTree tasks){
  fprintf(hFile, "%s%s%s",
      "/************************************************************",
      "\n   Function prototypes\n",
      "*************************************************************/\n\n");
  createPrototype(tasks->Node.Tasks.hard_tasks);
  createPrototype(tasks->Node.Tasks.soft_tasks);

  fprintf(hFile, "%s%s%s",
      "/************************************************************",
      "\n   End Function prototypes\n",
      "*************************************************************/\n\n");
}

/****************************************************
   Name:      createPrototype()
   Input:     task, a task in the syntax tree
   Output:    -
   Descr:     Generate the function protopypes for the
              start and error routine of the task, if
          they are not already generated
   Calls:     isInlist(),
              createPrototype(), recursive
*****************************************************/
void createPrototype(tTree task){
  char startAddress[MAX_STRING_LENGTH];
  char errorHandler[MAX_STRING_LENGTH];

  if(task){
    /* Get the startAddress and errorHandler id string */
    strcpy(startAddress, task->Node.Task.routine);

    /* Check if the routines are already declared, if not
       do it */
    if(!isInList(startAddress, &routine_list)){
      fprintf(hFile, "%s%s%s",
          "extern C_task void ", startAddress, "(void *);\n");
    }

    if(task->Node.Task.err_routine){
      strcpy(errorHandler, task->Node.Task.err_routine->Node.StringConst.Value);
      if(! isInList(errorHandler, &error_routine_list)){
    fprintf(hFile, "%s%s%c%s%s",
        "extern C_task void ", errorHandler, '(', ERRORTYPE, ");\n");
      }
    }
    else{
      if(!error_H_flag){
    error_H_flag = 1;
    fprintf(hFile, "%s%s%s", "extern void errorH(", ERRORTYPE, ");\n");
      }
    }


    /* Go for the next task */
    createPrototype(task->Node.Task.Next);
  }
}


/****************************************************
   Name:      createTCBList
   Usage:
   Arguments: tasks, the tasks branch of a mode in the
              syntax tree.
   Returns:   -
   Purpose:   Generates code for the TCB list
   Use:       countTasks(),
              createTaskInTCBList()
*****************************************************/
void createTCBList(tTree tasks){
    int noTasks = 0, is_soft_task = 0;;

        /* count number of tasks */
        noTasks = countTasks(tasks->Node.Tasks.hard_tasks);
        noTasks += countTasks(tasks->Node.Tasks.soft_tasks);

        /* write the header */
        fprintf(hFile, "%s%s%s%s%s%s",
            "/************************************************************",
            "\n   TCB list",
            "\n   T, O, DL, void (*startAddress)(void*), void *param,",
            "\n   void (*errorHandler)(int), stackpointer, CCR default value,",
            "\n   pointer to list of taken semaphores\n",
            "*************************************************************/\n\n");

        fprintf(cFile, "%s%s%s%s%s%s",
            "/************************************************************",
            "\n   TCB list",
            "\n   T, O, DL, void (*startAddress)(void*), void *param,",
            "\n   void (*errorHandler)(int), stackpointer, CCR default value,",
            "\n   pointer to list of taken semaphores\n",
            "*************************************************************/\n\n");


        fprintf(fpscalcFile,"%s%s" , 
                "\n  declarations {",
                "\n    tasks ");


        fprintf(hFile, "%s%s%s%s%d%s",
		"extern ",
		"TCB_t TCBList_", 
		mode_name, 
		"[",
		noTasks,
		"];\n\n");

        fprintf(cFile, "%s%s%s%d%s",
		"TCB_t TCBList_"
		,mode_name,
		"[",
		noTasks,
		"] = {\n");

        /* generate code for each task, booth hard and soft ones */
        if(tasks->Node.Tasks.soft_tasks)
            is_soft_task = 1;

        createTaskInTCBList(tasks->Node.Tasks.soft_tasks);

        if(is_soft_task && tasks->Node.Tasks.hard_tasks)
            fprintf(cFile, "%s", ",\n");

        createTaskInTCBList(tasks->Node.Tasks.hard_tasks);


        fprintf(fpscalcFile, "%s%s",
                "\n    indexed T,C,R,D,U;",
                "\n    priority P;\n  }\n");

        fprintf(hFile, "%c", '\n');
        fprintf(hFile, "%s%s%s",
            "/************************************************************",
            "\n   End TCB list\n",
            "*************************************************************/\n\n");

        fprintf(cFile, "%s", "};\n\n");
        fprintf(cFile, "%s%s%s",
            "/************************************************************",
            "\n   End TCB list\n",
            "*************************************************************/\n\n");
}

/****************************************************
   Name:      createTaskInTCBList()
   Input:     task, a task in the syntax tree
   Output:    -
   Descr:     Generates code for all tasks in the
              TCB list recursivley.
   Calls:     createTaskInTCBList(), recursive
*****************************************************/
void createTaskInTCBList(tTree task){
    char startAddress[MAX_STRING_LENGTH];
    char errorHandler[MAX_STRING_LENGTH];
    char arguments[MAX_STRING_LENGTH];
    char wf_buf[MAX_STRING_LENGTH+15];
    char sem_buf[MAX_STRING_LENGTH+22];
    int period = 0;

    static char taskname_string[65535]={0};
    static unsigned int string_offset = 0;


        if(task){
            strcpy(startAddress, task->Node.Task.routine);

            if(task->Node.Task.err_routine)
                strcpy(errorHandler, task->Node.Task.err_routine->Node.StringConst.Value);
            else
                strcpy(errorHandler, DEF_ERROR_ROUTINE);

            if(task->Node.Task.argument)
                strcpy(arguments, task->Node.Task.argument->Node.StringConst.Value);
            else
                strcpy(arguments, "NULL");

            if(wf_flag)
                sprintf(wf_buf, "&%s_bufctrl_%s[0]", task->Node.Task.name,mode_name);
            else
                sprintf(wf_buf, "NULL");

            if(task->Node.Task.semaphore_flag)
                sprintf(sem_buf, "task_%s_semaphoretaken_%s", task->Node.Task.name,mode_name);
            else
                sprintf(sem_buf, "NULL");

            /*------------------------------------------------------------------*/
            /* Generate the code that defines a task in the TCB list, that is:  */
            /* T, O, DL, void(*startAddress)(void*), void *param,               */
            /* void (*errorHandler)(int), stackpointer, CCR default value,      */
            /* waitfree buf control, pointer to list of taken semaphores        */
            /*------------------------------------------------------------------*/
            if(task->Node.Task.activator == NULL)
                period = task->Node.Task.period;

            if(!strcmp(arguments, "NULL"))
              {
                fprintf(cFile, "%c%d%c%d%c%d%c%s%s%s%s%s%c%s%c%s,%s%c",
                        '{', period, ',',
                        task->Node.Task.offset, ',', task->Node.Task.deadline, ',',
                        startAddress, ",", arguments, ",(errorfunc_t)",
                        errorHandler,
                        ',', "DEFAULT_CCR", ',', wf_buf,sem_buf,'}');
              }
            else
              {
                fprintf(cFile, "%c%d%c%d%c%d%c%s%s%s%s%s%c%s%c%s,%s%c",
                        '{', period, ',',
                        task->Node.Task.offset, ',', task->Node.Task.deadline, ',',
                        startAddress, ",&", arguments, ",(errorfunc_t)",
                        errorHandler,
                        ',', "DEFAULT_CCR", ',', wf_buf,sem_buf,'}');
              }
              
            if(task->Node.Task.Next != NULL)
              fprintf(cFile, "%s", ",\n");

            /* Printout the task's name to the FPS-Calc file */
            if (task->Node.Task.period >= 0  )
              {

                sprintf(taskname_string + string_offset, "%s, ",task->Node.Task.name);
                string_offset += (strlen(task->Node.Task.name) + 2);
              }

            if( task->Node.Task.Next == NULL  )
              {
                sprintf(taskname_string + string_offset - 2, "%s", "; ");
                fprintf(fpscalcFile, "%s ",taskname_string);
              }
            /* Go for the next task */
            createTaskInTCBList(task->Node.Task.Next);
        }
}



/****************************************************
   Name:      closeFiles()
   Usage:
   Arguments: -
   Returns:   -
   Purpose:   Closes the file generated.
   Use:       -
*****************************************************/
void closeFiles(void){
  fclose(cFile);
  fclose(hFile);
  fclose(incFile);
  fclose(fpscalcFile);
  fclose(boundtFile);
}
