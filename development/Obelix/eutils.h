/*
   File:     eutils.h
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 1.0, 00/04/18
   Desc:     Help routines for the emit routines.
*/

/* List element for storing name of routines */
typedef struct _NameListEl{
  char name[MAX_STRING_LENGTH];
  struct _NameListEl *next;
}NameListEl;

/* List element for storing waitfree data */
typedef struct _wfel{
  int index;
  int is_writer;
  struct _wfel *next;
}wfel;

unsigned int countTasks(tTree tasks);
unsigned int countSignals(tTree signals);
unsigned int have_signals( tTree mode);
unsigned int countSemaphores(tTree semaphores);
unsigned int have_semaphores( tTree mode);
unsigned int countWaitfree(tTree waitfree);
unsigned int have_waitfree( tTree mode);
wfel *getWfEl(int index, int is_writer);
void freeWfElList(wfel **);
int isWriter(tTree task, tTree wf);
int checkWf(tTree task, tTree wf);
int isInList(char name[], NameListEl **list);
NameListEl *getListEl(char name[]);
void freeNameList(NameListEl **list);
int countUsers(tTree semaphore);
int getUserTaskListIndex(tTree user, tTree task_list[],
			 int no_tasks );
int getSemaphoreTaskListIndex(tTree semaphore, tTree task_list[],
			      int no_tasks );

