/********************************************
  Name:     semact.c
  Author:   Joakim Lindgren, Anders Davidsson
  Revision: 02/05/2000  sort_list(tTree *list), adjusted to generate less
                        warings when eqal priorities are given by the user.  
            31/03/2000  Reviewed.
            25/01/2000  Finnished coded exept the semaphore single type of
                        task as argument.
            26/01/2000  Have added some more checks.
  Descr:    This file contains some sematic controlls on a syntax tree:

  	    - One controll for signals as trigger for tasks, but also checks
	      that the task is in the signal definition.

            - One for task as argument in resources.

            - One for semaphores. Checks that task is of same type and find
              the ceiling prio.

            - One that checks all tasks and make all priorites unique.

            - One control for waitfree, checks the number of bufferts and
              number of users.

            - A stacksize controll, if the assigned stacksize is less than
              10, a warning is generated.
********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntree.h"
#include "semact.h"
#include "Obelix.h"

/* Please, comment away if no trace print is wanted. */
/*#define TRACE_SEMANTIC*/

#ifdef TRACE_SEMANTIC
  #define trace_semantic(TRACE_SEMANTIC_STRING)\
    {\
      printf("%s", TRACE_SEMANTIC_STRING);\
    }
#else
  #define trace_semantic(TRACE_SEMANTIC_STRING) { }
#endif


#define kHARDTASK 1
#define kSOFTTASK 2



static tTree find_task (tTree mode, char *task, int *Kind);
static tTree find_signal (tTree mode, char *signal);
static void find_user (tTree signal, char *name);

static int count_user (tTree user);

static void checkStackSize (tTree task);

static void swap (tTree * ptr);
static void semaphoreSwap (tTree * ptr);


static void sort_list (tTree * list);
static void sortSemaphores (tTree * list);

static void assign_new_priorities (tTree soft_task, tTree hard_task, tTree semaphores);


/*int type_error=0;*/



/**********************************
  Name:   checkStacks
  Input:  tTree program, a complete syntax tree
  Output: -
  Calls:  checkStackSize
  Descr:  Checks all user stacks.
**********************************/
void 
checkStacks (tTree program)
{
  tTree mode = program->Node.Program.mode;

    while (mode)
    {
      checkStackSize (mode->Node.Mode.tasks->Node.Tasks.hard_tasks);
      checkStackSize (mode->Node.Mode.tasks->Node.Tasks.soft_tasks);
      mode = mode->Node.Mode.Next;
    }
}


/********************************
  Name:   checkStackSize
  Input:  tTree program, a complete syntax tree
  Output: -
  Calls:  checkStackSize, recursively
  Descr:  Checks the size of user stacks and gives a warning if
          less than 10.
********************************/
void 
checkStackSize (tTree task)
{
    if (task)
    {
      if (task->Node.Task.stack < MIN_STACK_SIZE)
      {
        noWarnings++;
        fprintf (stderr, "Warning at line: %d, size of stack may not be enough.\n",task->LineNr);
      }
    }
}


/*********************************
  Name:   check_signal_arg
  Input:  tTree program, a complete syntax tree
  Output: -
  Calls:  find_signal
          find_user
  Descr:  Checs if signals passed as arguments, really
          are signals. 
*********************************/
void
check_signal_arg (tTree program)
{
  tTree temp_mode;
  tTree temp_task;
  tTree temp, temp2;

    temp_mode = program->Node.Program.mode;

    while (temp_mode != NULL)
    {
      temp = temp_mode->Node.Mode.tasks;
      temp_task = temp->Node.Tasks.hard_tasks;

      while (temp_task != NULL)	/* hard tasks */
      {
        trace_semantic ("Kollar ett hårt task.\n");

        if (temp_task->Node.Task.activator != NULL)
        {
          temp = temp_task->Node.Task.activator;

          if (temp->Kind == kStringConst)
          {
            trace_semantic("Kollar efter en signal för ett hårt task.\n");

            temp2 = find_signal (temp_mode, temp->Node.StringConst.Value);
            if (temp2 == NULL)
            {
              printf ("Error at line: %d, mismatch type.\n",temp->LineNr);
              noErrors++;
            }
            else
            {
              find_user (temp2, temp_task->Node.Task.name);
            }
          }
        }

        temp_task = temp_task->Node.Task.Next;
      }

      temp = temp_mode->Node.Mode.tasks;
      temp_task = temp->Node.Tasks.soft_tasks;
      while (temp_task != NULL)	/* soft tasks */
      {
        trace_semantic ("Kollar ett mjukt task.\n");

        if (temp_task->Node.Task.activator != NULL)
        {
          temp = temp_task->Node.Task.activator;
          if (temp->Kind == kStringConst)
          {
            trace_semantic("Kollar efter en signal för ett mjukt task.\n");
            temp2 = find_signal(temp_mode, temp->Node.StringConst.Value);
            if (temp2 == NULL)
            {
              printf ("Error at line: %d, mismatch type\n",temp->LineNr);
              noErrors++;
            }
            else
            {
              find_user(temp2, temp_task->Node.Task.name);
            }
          }
        }

        temp_task = temp_task->Node.Task.Next;
      }

      temp_mode = temp_mode->Node.Mode.Next;
    }
}


/***************************
  Name:   find_signal
  Input:  tTree mode, a mode branch of the syntax tree
          char *signal, a signal name
  Output: tTree, the matching signal node.
  Calls:  -
  Descr:  looks for a signal definition that match the input argument
***************************/
tTree 
find_signal (tTree mode, char *signal)
{
  tTree temp;

    temp = mode->Node.Mode.resources->Node.Resources.synchronizations;
    temp = temp->Node.Synchronizations.signals;

    while (temp != NULL)
    {
      if (strcmp (temp->Node.Signal.name, signal) == 0)
        return temp;

      temp = temp->Node.Signal.Next;
    }

    return NULL;
}


/***************************
  Name:   find_user
  Input:  tTree signal, a signal node in the syntax tree
          char *name, a task id
  Output: -
  Calls:  -
  Descr:  Looks for name among the users of signal, if it
          is not found an error is generated.
***************************/
void
find_user (tTree signal, char *name)
{
  tTree temp;

    temp = signal->Node.Signal.user;

    while (temp != NULL)
    {
      if (strcmp (temp->Node.User.string, name) == 0)
        return;

      temp = temp->Node.User.Next;
    }

    fprintf (stderr, "Error at line: %d, task %s not user.\n", signal->LineNr,name);
    noErrors++;

}



/****************************
  Name:   check_task_arg
  Input:  tTree program, a complete syntax tree
  Output: -
  Calls:  trace_semantic
          find_task	  
  Descr:  Checks that all task argument is of type "task".
****************************/
void
check_task_arg (tTree program)
{
  tTree temp_mode;
  tTree temp_res;
  tTree temp;
  int dummy;

    temp_mode = program->Node.Program.mode;

    while (temp_mode != NULL)
    {
      trace_semantic ("Kollar ett mode.\n");

      temp_res = temp_mode->Node.Mode.resources->Node.Resources.communications;
      temp_res = temp_res->Node.Communications.wait_free;
      while (temp_res != NULL)	/* for all wait_frees */
      {
        trace_semantic ("Kollar ett WaitFree.\n");

        if (find_task (temp_mode, temp_res->Node.WaitFree.writer, &dummy) == NULL)
        {
          printf ("Mismatch type in line %d\n", temp_res->LineNr);
          noErrors++;
        }

        temp = temp_res->Node.WaitFree.reader;

        while (temp != NULL)
        {
          if (find_task (temp_mode, temp->Node.User.string, &dummy) == NULL)
          {
            printf ("Error at line: %d, mismatch type.\n",temp->LineNr);
            noErrors++;
          }

          temp = temp->Node.User.Next;
        }

        temp_res = temp_res->Node.WaitFree.Next;
      }


      temp_res = temp_mode->Node.Mode.resources->Node.Resources.synchronizations;
      temp_res = temp_res->Node.Synchronizations.signals;
      while (temp_res != NULL)
      {
        temp = temp_res->Node.Signal.user;

        while (temp != NULL)
        {
          if (find_task (temp_mode, temp->Node.User.string, &dummy) == NULL)
          {
            printf ("Error at line: %d, mismatch type.\n",temp->LineNr);
            noErrors++;
          }

          temp = temp->Node.User.Next;
        }

        temp_res = temp_res->Node.Signal.Next;
      }


      temp_res = temp_mode->Node.Mode.resources->Node.Resources.synchronizations;
      temp_res = temp_res->Node.Synchronizations.semaphores;
      while (temp_res != NULL)
      {
        temp = temp_res->Node.Semaphore.user;
        while (temp != NULL)
        {
          if( find_task(temp_mode, temp->Node.User.string, &dummy) == NULL)
          {
            fprintf (stderr, "Error at line: %d, mismatch type.\n",temp->LineNr);
            noErrors++;
          }

          temp = temp->Node.User.Next;
        }

        temp_res = temp_res->Node.Semaphore.Next;
      }


      /**************** 
        Sort booth task-branches by the priority 
        warn if the user has assigned equal 
        priorities for several tasks.
       ****************/
  
      sort_list (&(temp_mode->Node.Mode.tasks->Node.Tasks.soft_tasks));
      sort_list (&(temp_mode->Node.Mode.tasks->Node.Tasks.hard_tasks));

      temp_mode = temp_mode->Node.Mode.Next;
    }
}






/*********************************
  Name:   find_task
  Input:  tTree mode, a mode branch of the syntax tree
	        char *task, a task id to find
	        int *Kind, a pointer to where to put type.
  Output: tTree, a pointer to the task found or NULL
  Calls:  -
  Descr:  Looks for a task defintion in mode that match the input 
          argument "task".
**************************************/
tTree 
find_task(tTree mode, char *task, int *Kind)
{
  tTree temp;

    temp = mode->Node.Mode.tasks->Node.Tasks.hard_tasks;

    while (temp != NULL)
    {
      if (strcmp (temp->Node.Task.name, task) == 0)
      {
        *Kind = kHARDTASK;
        return temp;
      }
      temp = temp->Node.Task.Next;
    }

    temp = mode->Node.Mode.tasks->Node.Tasks.soft_tasks;

    while (temp != NULL)
    {
      if (strcmp (temp->Node.Task.name, task) == 0)
      {
        *Kind = kSOFTTASK;
        return temp;
      }

      temp = temp->Node.Task.Next;
    }

    return NULL;
}



/****************************
  Name:   check_semaphores
  Input:  tTree program, a program syntax tree
  Output: -
  Calls:  find_task
  Descr:  Checks that one semaphore is not in use by
          both hard and soft tasks and fills in the
	  priority ceiling for the semaphore.
****************************/
void
check_semaphores (tTree program)
{
  tTree temp_mode;
  tTree temp_res;
  tTree temp, temp2;
  int Kind, temp_kind;
  int max_prio = 0;

    temp_mode = program->Node.Program.mode;

    /*********** 
      For each mode 
    ***********/
    while (temp_mode)
    {

      temp_res = temp_mode->Node.Mode.resources->Node.Resources.synchronizations;
      temp_res = temp_res->Node.Synchronizations.semaphores;

      /*********
        For each semaphore
      *********/
      while (temp_res != NULL)
      {

        /************
          Get the first user
        ************/
        temp = temp_res->Node.Semaphore.user;

        /***********
          Find user task
        ***********/
        temp2 = find_task(temp_mode, temp->Node.User.string, &Kind);

        if (temp2 != NULL)
        {
          /*************
            Get user prio
          **************/
          max_prio = temp2->Node.Task.priority;

          /*************
            Assign task priority to user
          *************/
          temp->Node.User.prio = temp2->Node.Task.priority;

          /************
            Move on to the next user
          ************/
          temp = temp->Node.User.Next;
        }
        else
        {
          fprintf (stderr,"Error at line: %d,  undefined task in semaphore.\n",temp->LineNr);
          noErrors++;
        }

        /* for all users */
        while (temp != NULL)
        {

          /* find user task */
          temp2 = find_task(temp_mode, temp->Node.User.string, &temp_kind);

          if (temp2 != NULL)
          {
            /* check task kind, hard - soft */
            if (Kind != temp_kind)
            {
              fprintf (stderr,"Error at line: %d, missmatch type, only one type of task is allowed to use the same semaphore\n",temp->LineNr);
              noErrors++;
            }
            else if (max_prio < temp2->Node.Task.priority)
              max_prio = temp2->Node.Task.priority;

            /* assign task priority to user */
            temp->Node.User.prio = temp2->Node.Task.priority;
          }
          else
          {
            fprintf (stderr,"Error at line: %d,  undefined task in semaphore.\n",temp->LineNr);
            noErrors++;
          }

          temp = temp->Node.User.Next;
        }

        /* fill in the priority ceiling */
        temp_res->Node.Semaphore.ceil = max_prio;


        temp_res = temp_res->Node.Semaphore.Next;
      }

      /* */
      sortSemaphores( &(temp_mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores));
      //printTree(temp_mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores);


      /* Adjust the priorities with respect to the priority ceilings of the semaphores */
      assign_new_priorities ( temp_mode->Node.Mode.tasks->Node.Tasks.soft_tasks,
                              temp_mode->Node.Mode.tasks->Node.Tasks.hard_tasks,
                              temp_mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores );


      /* Now it is time for the next Mode */
      temp_mode = temp_mode->Node.Mode.Next;
    }
}



/************************************
  Name:   make_unique_prio
  Input:  tTree program, a program syntax tree
  Output: -
  Calls:  sort_list
          assign_new_priorities

  Descr:  Checks that all task has unique priorities, cause the
          kernel use them as id's. The hard tasks priorities is
          always higher than the soft tasks, and if two tasks 
          has the same id, a warning is fired.

          The actual priorities are calculatded so that all tasks 
          will get unique priorities, both hard and soft.
				
*************************************/
void
make_unique_prio (tTree program)
{
 
  tTree temp_mode;
  tTree hard_tasks;
  tTree soft_tasks;



  temp_mode = program->Node.Program.mode;

  while (temp_mode)
    {
  
      soft_tasks = temp_mode->Node.Mode.tasks->Node.Tasks.soft_tasks;
      hard_tasks = temp_mode->Node.Mode.tasks->Node.Tasks.hard_tasks;
  
      /***************** 
        Assign task priorities that follow 
        each other start with soft tasks and the 
        lowest user assigned priority.
       *****************/
  
      assign_new_priorities(soft_tasks, hard_tasks, NULL);
  
      /*****************
        Now it is time for the next Mode
       *****************/
      temp_mode = temp_mode->Node.Mode.Next;
    }
}


/*************************
  Name:   assign_new_priorities
  Input:  tTree soft_task, soft task branch
          tTree hard_task, hard task branch
  Output: -
  Calls:  -
  Descr:  Assign system priorities to all tasks.

*************************/
void
assign_new_priorities (tTree soft_tasks, tTree hard_tasks, tTree semaphores )
{
  int i = 0, j=0, k=0;
  tTree tmp_soft = soft_tasks;
  tTree tmp_hard = hard_tasks;
  tTree tmp_sem  = semaphores;



    /* Assigning unique priorities to all soft tasks with respect to the semaphores priority ceiling */
    while ( tmp_soft != NULL)
    {
      tmp_soft->Node.Task.priority = i;
      i++;
      if( tmp_sem != NULL )
      {
	j = 0;
	while( (tmp_sem != NULL ) && (tmp_sem->Node.Semaphore.ceil+k == tmp_soft->Node.Task.priority) )
        {
	  tmp_sem->Node.Semaphore.ceil = i;
          i++; j++;
	  tmp_sem = tmp_sem->Node.Semaphore.Next;
        }
	k = k+j;
      }
      tmp_soft = tmp_soft->Node.Task.Next;
    }


    /* Assigning unique priorities to all hard tasks with respect to the semaphores priority ceiling */
    while ( tmp_hard != NULL)
    {
      tmp_hard->Node.Task.priority = i;
      i++;
      if( tmp_sem != NULL )
      {
	j = 0;
	while( (tmp_sem != NULL) && (tmp_sem->Node.Semaphore.ceil+k == tmp_hard->Node.Task.priority) )
        {
	  tmp_sem->Node.Semaphore.ceil = i;
          i++; j++;
	  tmp_sem = tmp_sem->Node.Semaphore.Next;
        }
	k = k+j;
      }
      tmp_hard = tmp_hard->Node.Task.Next;
    }

    /* Adjust the priority ceiling of the remaining semaphores */
    while( tmp_sem != NULL )
    {
      tmp_sem->Node.Semaphore.ceil = i;
      i++;
      tmp_sem = tmp_sem->Node.Semaphore.Next;
    }


}


/***************************************
  Name:   sort_list
  Input:  tTree *list, a double pointer to a list of task treeNodes
  Output: -
  Calls:  swap()
  Descr:  Sorts (bubble sort) a list of task nodes by the user assigned task
          priority, low priority first.
				
***************************************/
void
sort_list (tTree * list)
{
  tTree temp = *list;
  tTree *temp2 = list;
  int length = 0, prev_prio = 0;
  int i;

    if(*list == NULL || (*list)->Node.Task.Next == NULL)  /* no need to sort */
      return;

    /* Count the number of tasks in the list */
    while (temp)
    {
      length++;
      temp = temp->Node.Task.Next;
    }

    /* bubble sort the list :) */
    for (i = 0; i < length - 1; i++)
    {
      temp2 = list;

      while ((*temp2)->Node.Task.Next)
      {
        if( (*temp2)->Node.Task.priority > (*temp2)->Node.Task.Next->Node.Task.priority)
          swap (temp2); /* swap the two following nodes */

        temp2 = &((*temp2)->Node.Task.Next);
      }
    }

    /* Warn if there are equal prioritys assigned in the system */
    temp = *list;

    prev_prio = temp->Node.Task.priority;
    temp = temp->Node.Task.Next;

    while (temp)
    {
      if (prev_prio == temp->Node.Task.priority)
      {
        fprintf (stderr,"%s Warning at line: %d, task priorities are same for tasks.\n",temp->Node.Task.name, temp->LineNr);
        noWarnings++;
      }

      prev_prio = temp->Node.Task.priority;
      temp = temp->Node.Task.Next;
    }
}

/*********************
  Name:   sortSemaphores
  Input:  tTree *list, a double pointer to a list of semphore treeNodes
  Output: -
  Calls:  semphoreSwap
  Descr:  Sorts (bubble) a list of semphore nodes by their ceiling,
          low ceiling first.
				
*********************/
void
sortSemaphores (tTree * list)
{
  tTree *temp = list;
  tTree temp2 = *list;
  int length = 0;
  int i;

    if(*list == NULL || (*list)->Node.Semaphore.Next == NULL) /* no need to sort */
      return;

    while (temp2)
    {
      length++;
      temp2 = temp2->Node.Semaphore.Next;
    }

    for (i = 0; i < length - 1; i++)
    {
      temp = list;

      while ((*temp)->Node.Semaphore.Next)
      {
        if( (*temp)->Node.Semaphore.ceil > (*temp)->Node.Semaphore.Next->Node.Semaphore.ceil )
          semaphoreSwap(temp); /* swap the two following nodes */

        temp = &((*temp)->Node.Semaphore.Next);
      }
    }
}


/*************************
  Name:   semaphoreSwap
  Input:  tTree *ptr, a double pointer to a Semaphore treeNode
  Output: -
  Calls:  -
  Descr:  Swaps the semaphore node with the node after it.
				
*************************/
void
semaphoreSwap (tTree * ptr)
{
  tTree temp;

    temp = (*ptr)->Node.Semaphore.Next;	/* save the pointer to the second link */

    (*ptr)->Node.Semaphore.Next = (*ptr)->Node.Semaphore.Next->Node.Semaphore.Next;

    temp->Node.Semaphore.Next = (*ptr);

    *ptr = temp;
}


/*************************
  Name:   swap
  Input:  tTree *ptr, a double pointer to a task treeNode
  Output: -
  Calls:  -
  Descr:  Swaps the task node with the node after it.
				
*************************/
void
swap (tTree * ptr)
{
  tTree temp;

    /*printf("Enter swap\n"); */
    temp = (*ptr)->Node.Task.Next;	/* save the pointer to the second link */

    (*ptr)->Node.Task.Next = (*ptr)->Node.Task.Next->Node.Task.Next;

    temp->Node.Task.Next = (*ptr);

    *ptr = temp;
}



/**************************
  Name:   checkWaitFree
  Input:  tTree program, a program syntax tree
  Output: -
  Calls:  count_user
  Descr:  This routine checks that the given buffer size is enough, else error
          if no value is given a default is set. 
**************************/
void
check_wait_free (tTree program)
{
  tTree temp_mode;
  tTree temp_res;
  int user;

    /*  printf("Enter check_wait_free\n"); */
    temp_mode = program->Node.Program.mode;

    while (temp_mode)
    {

      temp_res = temp_mode->Node.Mode.resources->Node.Resources.communications;
      temp_res = temp_res->Node.Communications.wait_free;

      while (temp_res != NULL)
      {

        user = count_user (temp_res->Node.WaitFree.reader);
        //printf("users: %d\n", user);

        if( temp_res->Node.WaitFree.buffers == -1)
          temp_res->Node.WaitFree.buffers = 2 + user;
        else if (temp_res->Node.WaitFree.buffers < 2 + user)
        {
          fprintf (stderr,"Warning at line: %d, assigned buffer number is to small.\n",temp_res->LineNr);
          temp_res->Node.WaitFree.buffers = 2 + user;
          noWarnings++;
        }

        temp_res = temp_res->Node.WaitFree.Next;
      }

      temp_mode = temp_mode->Node.Mode.Next;
    }
}


/********************************
  Name:   count_user
  Input:  tTree user, a user branch of a waitfree node
  Output: int, the number of users
  Calls:  -
  Descr:  Counts the number of users of a watifre channel
********************************/
int
count_user (tTree user)
{

    if (user == NULL)
      return 0;
    else
      return 1 + count_user (user->Node.User.Next);

}
