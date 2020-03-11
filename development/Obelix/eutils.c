/*
   File:     eutils.c
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 1.0, 00/04/18
   Desc:     Help routines for the emit routines.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "syntree.h"
#include "eutils.h"


/****************************************************
   Name:      countTasks()
   Input:     tTree tasks, a task branch of the ST.
   Output:    Number of tasks in the tasks branch.
   Descr:     Counts the number of tasks in the brach.
   Calls:     -
*****************************************************/
unsigned int countTasks(tTree tasks){
  int noOfTasks = 1;

  if(!tasks)
  return 0;
  
  while(tasks->Node.Task.Next){
    noOfTasks++;
    tasks = tasks->Node.Task.Next;
  }

  return noOfTasks;
} 


/****************************************************
   Name:      countSignals()
   Input:     tTree signals, a signal branch of the ST.
   Output:    Number of signals in the signal branch.
   Descr:     Counts the number of signals in the brach.
   Calls:     -
*****************************************************/
unsigned int countSignals(tTree signals){
  int noOfSignals = 1;

  if(!signals)
  return 0;
  
  while(signals->Node.Signal.Next){
    noOfSignals++;
    signals = signals->Node.Signal.Next;
  }

  return noOfSignals;
} 
 



/****************************************************
   Name:      countSemaphores()
   Input:     tTree sempahores, a semaphore branch of the ST.
   Output:    Number of semaphores in the semaphore branch.
   Descr:     Counts the number of semaphores in the brach.
   Calls:     -
*****************************************************/
unsigned int countSemaphores(tTree semaphores){
  int noOfSemaphores = 1;

  if(!semaphores)
  return 0;
  
  while(semaphores->Node.Semaphore.Next){
    noOfSemaphores++;
    semaphores = semaphores->Node.Semaphore.Next;
  }

  return noOfSemaphores;
} 


/****************************************************
   Name:      countWaitfree()
   Input:     tTree Waitfree, a waitfree communication channel branch of the ST.
   Output:    Number of waitfree channels in the waitfree channel branch.
   Descr:     Counts the number of waitfree channels in the brach.
   Calls:     -
*****************************************************/
unsigned int countWaitfree(tTree waitfree){
  int noOfWaitfree = 1;

  if(!waitfree)
  return 0;
  
  while(waitfree->Node.WaitFree.Next){
    noOfWaitfree++;
    waitfree = waitfree->Node.WaitFree.Next;
  }

  return noOfWaitfree;
} 


unsigned int have_signals( tTree mode ){
  return (unsigned int)mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.signals;
}

unsigned int have_semaphores( tTree mode ){
  return (unsigned int)mode->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores;
}

unsigned int have_waitfree( tTree mode ){
  return (unsigned int)mode->Node.Mode.resources->Node.Resources.communications->Node.Communications.wait_free;
}

/****************************************************
   Name:      getWfEl()
   Input:     int index,
              int is_writer
   Output:    A pointer to the new elemet.
   Descr:     Allocates memory for a wfel
   Calls:     -
*****************************************************/
wfel *getWfEl(int index, int is_writer){
  wfel *temp;

  temp = (wfel *)malloc(sizeof(wfel));
  if(!temp){
    printf("Out of memory!\n");
    exit(0);
  }

  temp->index = index;
  temp->is_writer = is_writer;
  temp->next = NULL;
 
  return temp;
}

/****************************************************
   Name:      freeWfElList()
   Input:     wfel **list, a list of wfel
   Output:    -
   Descr:     Free space.
   Calls:     -
*****************************************************/
void freeWfElList(wfel **list){
  wfel *temp;
  
  while(*list){
    temp = *list;
    *list = (*list)->next;
    free(temp);
  }
}

/****************************************************
   Name:      checkWf()
   Input:     tTree task, a task node
              tTree wf, a waitfree node of the ST.
   Output :   1 or 0.
   Discr:     Decides if task is user of wf
   Calls:     -
*****************************************************/
int checkWf(tTree task, tTree wf){
  tTree readers = NULL;

  if(!strcmp(wf->Node.WaitFree.writer, task->Node.Task.name))
    return 1;
  readers = wf->Node.WaitFree.reader;
  while(readers){
    if(!strcmp(readers->Node.User.string, task->Node.Task.name))
      return 1;
    readers = readers->Node.User.Next;
  }

  return 0;
}


/****************************************************
   Name:      isWriter()
   Input:     tTree task, a task node
              tTree wf, a waitfree node of the ST.
   Output :   1 or 0.
   Discr:     Decides if task is writer of wf
   Calls:     -
*****************************************************/
int isWriter(tTree task, tTree wf){
  return !strcmp(wf->Node.WaitFree.writer, task->Node.Task.name);
}


/****************************************************
   Name:      isInList()
   Usage:
   Arguments: name, a string
              list, a list of NameList elements
   Returns:   0 if the name is not in the list,
              1 it the name is in the list
   Purpose:   Checks if name is in list, if it is not
              a new list element is created and put
	      at the end of the list.
   Use:       getListEl()
*****************************************************/
int isInList(char name[], NameListEl **list){
  int isInList = 1;
  
  /*printf("Enter isInList() %s\n", name);*/
  /*printList(*list);*/

  if(!*list){
    /* if the list was empty */
    *list = getListEl(name);
    isInList = 0;
  }
  else{
    /* look for the name */
    while(strcmp((*list)->name, name) != 0){
      if(!(*list)->next){
	/* end of list, name was not included,
	   get a new NameListEl */
	(*list)->next = getListEl(name);
	*list = (*list)->next;
	isInList = 0;
      }
      else
	*list = (*list)->next;
    }
  }
  /*printf("Leaving isInList()\n");*/
  return isInList;
}

/****************************************************
   Name:      getListEl()
   Usage:
   Arguments: name, a string
   Returns:   a pointer to a new NameListEl
   Purpose:   Allocates memory and fills in name of
              a new NameListEl
   Use:       -
*****************************************************/
NameListEl *getListEl(char name[]){
  NameListEl *temp;

  temp = (NameListEl *)malloc(sizeof(NameListEl));
  if(!temp){
    printf("Out of memory!\n");
    exit(0);
  }
  strcpy(temp->name, name);
  temp->next = NULL;
  
  return temp;
}

/****************************************************
   Name:      freeNameList()
   Usage:
   Arguments: list, a list of NameListEl
   Returns:   -
   Purpose:   Free all elements in the list
   Use:       -
*****************************************************/
void freeNameList(NameListEl **list){
  NameListEl *temp;
  
  while(*list){
    temp = *list;
    *list = (*list)->next;
    free(temp);
  }
}


/****************************************************
   Name:      countUsers()
   Input:     tTree user, a user branch of the syntax tree
   Output :   The number of users.
   Discr:     Counts the number of users
   Calls:     -
*****************************************************/
int countUsers(tTree users){
  tTree temp = users;
  int count = 0;

  while(temp){
    count++;
    temp = temp->Node.User.Next;
  }

  return count;
}


/****************************************************
   Name:      getSemaphoreTaskListIndex()
   Input:     tTree semaphore, a semaphore branch of the syntax tree,
              tTree task_list[], the representation of the task list.
   Output:    The semaphore index in the task list, if not found
              -1 is returned.
   Discr:     Goes through the task list until semaphore is found.
   Calls:     -
*****************************************************/
int getSemaphoreTaskListIndex(tTree semaphore, tTree task_list[],
			      int no_tasks){
  int i;

  for(i = 0; i < no_tasks ; i++)
    if(task_list[i]->Kind == kSemaphore)
      if(!strcmp(task_list[i]->Node.Semaphore.name,
		semaphore->Node.Semaphore.name)){
	return i;
      }
  return -1;
}

/****************************************************
   Name:      getUserTaskListIndex()
   Input:     tTree user, a user branch
              tTree task_list[], the representation of the task list.
   Output:    The users index in the task list
   Discr:     Goes through the task list for user, user is a task (tcb).
   Calls:     -
*****************************************************/
int getUserTaskListIndex(tTree user, tTree task_list[], int no_tasks)
{
	int i;

		for(i = 1; i < no_tasks ; i++)
		{
			if(task_list[i]->Kind == kTask)
				if(!strcmp(task_list[i]->Node.Task.name,user->Node.User.string))
					return i;
		}
		return -1;
}


