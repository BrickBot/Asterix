/*
  File:     syntree.c

  Author:   Joakim Lingren, Anders Davidsson

  Descr:    A package support for creating syntax tree based on
            the Obelix confguration grammar.

  Revision: 30-03-2000   Reviewing.
            11-01-2000   The append functions is still left over to be coded. And the rest of the
                   code have not been verified to work proper.
            13-01-2000   A general append function is implemented.
            15-01-2000   Have make a printTree function the traverse the tree
                         and prints the nodes. A treeFree has also been made.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <syntree.h>

#define connectMacro(NODETYPE,LIST,ELEMENT){       \
                                 \
  tTree tmp = LIST;                                \
                           \
  if (tmp == NULL)                                 \
    return (ELEMENT);                        \
  else                                             \
    while (tmp->Node.NODETYPE.Next != NULL)  \
      tmp = tmp->Node.NODETYPE.Next;   \
               \
  tmp->Node.NODETYPE.Next = ELEMENT;               \
  return (LIST);                                   \
}


/*
  Name:   connectUser
  Input:  tTree pUsers, pointer to a list.
    tTree pUser, pointer to a node/element
    int kType, the kType of the nodes in the previous argument
  Output: tTree, the extended list
  Calls:  -
  Descr:  A general append, that adds the element last in the list
*/

tTree connectNode (tTree pUsers, tTree pUser, int kType)
{
  if (pUsers == NULL || pUser == NULL) {
    fprintf (stderr, "NO_NODE argument to connectActuals, exiting...\n\r");
    exit(EXIT_FAILURE);
  }
  if (pUsers->Kind != kType || pUser->Kind != kType) {
    fprintf (stderr, "Wrong node type in connectNode, %d %d exiting...\n\r",
       pUsers->Kind, pUser->Kind);
    exit(EXIT_FAILURE);
  }


switch(kType){
  case kMode:       connectMacro (Mode, pUsers, pUser);      break;
  case kTask:       connectMacro (Task, pUsers, pUser);      break;
  case kWaitFree:   connectMacro (WaitFree, pUsers, pUser);  break;
  case kSignal:     connectMacro (Signal, pUsers, pUser);    break;
  case kUser:       connectMacro (User, pUsers, pUser);      break;
  case kSemaphore:  connectMacro (Semaphore, pUsers, pUser); break;
  default: perror("Missmatch type in function :connectNode\n"); exit(EXIT_FAILURE);
  }
}

/*
  Name:   printTaskPrio()
  Input:  tTree root, a syntax tree
  Output: -
  Calls:  printTaskPrio, recursively
  Descr:  Prints the priorities of the tasks in the
          syntax tree.
*/
void printTaskPrio(tTree root){
  if(root){
    switch(root->Kind){
    case kProgram:
      printTaskPrio(root->Node.Program.mode);
      break;
    case kMode:
      printf("Mode: %s\n", root->Node.Mode.name);
      printTaskPrio(root->Node.Mode.tasks);
      printTaskPrio(root->Node.Mode.Next);
      break;
    case kTasks:
      printTaskPrio(root->Node.Tasks.hard_tasks);
      printTaskPrio(root->Node.Tasks.soft_tasks);
      break;
    case kTask:
      printf("Task %s prio: %d\n", root->Node.Task.name, root->Node.Task.priority);
      printTaskPrio(root->Node.Task.Next);
      break;
    }
  }
}

/*
  Name:   printSemaphoreCeil()
  Input:  tTree root, a syntax tree
  Output: -
  Calls:  printSemaphoreCeil, recursively
  Descr:  Prints the ceil of each semaphore in the
          syntax tree.
*/
void printSemaphoreCeil(tTree root){
  if(root){
    switch(root->Kind){
    case kProgram:
      printSemaphoreCeil(root->Node.Program.mode);
      break;
    case kMode:
      printf("Mode: %s\n", root->Node.Mode.name);
      printSemaphoreCeil(root->Node.Mode.resources->Node.Resources.synchronizations->Node.Synchronizations.semaphores);
      printSemaphoreCeil(root->Node.Mode.Next);
      break;
    case kSemaphore:
      printf("Semaphore %s ceil: %d\n", root->Node.Semaphore.name, root->Node.Semaphore.ceil);
      printSemaphoreCeil(root->Node.Semaphore.Next);
      break;
    }
  }
}


/*
  Name:   printTree
  Input:  tTree root, a syntax tree
  Output: -
  Calls:  printTree, recursively
  Descr:  Traverse the syntax-tree and print it.
*/

void printTree(tTree root)
{
  if(root==NULL)
    return;

  switch(root->Kind){
  case kProgram:
    printf("SystemMode: %d\n", root->Node.Program.systemMode);
    printf("Ram: %d\n", root->Node.Program.ram);
    printTree(root->Node.Program.mode);
    break;

  case kMode:
    printf("----------- Mode ------------\n");
    printf("Name: %s\n", root->Node.Mode.name);
    printf("Clock: %d\n", root->Node.Mode.clock);
    printTree(root->Node.Mode.tasks);
    printTree(root->Node.Mode.resources);
    printf("----------- Mode ------------\n");
    printTree(root->Node.Mode.Next);
    break;

  case kTask:
    printf("Task: \n");
    printf("----------- Task ------------\n");
    printf("Name: %s\n", root->Node.Task.name);
    printf("Activator: ");
    printTree(root->Node.Task.activator);
    printf("Offset: %d\n", root->Node.Task.offset);
    printf("Deadline: %d\n", root->Node.Task.deadline);
    printf("Priority: %d\n", root->Node.Task.priority);
    printf("Stack size: %d\n", root->Node.Task.stack);
    printf("Routine: %s\n", root->Node.Task.routine);
    printf("Arguments: ");
    printTree(root->Node.Task.argument);

//    printf("err_routine: %s\n", root->Node.Task.err_routine->Node.StringConst.Value);
printf("\n");
    printf("----------- Task ------------\n");
    printTree(root->Node.Task.Next);
    break;

  case kWaitFree:
    printf("----------- WaitFree ------------\n");
    printf("Name: %s\n", root->Node.WaitFree.name);
    printf("Writer: %s\n", root->Node.WaitFree.writer);
    printTree(root->Node.WaitFree.reader);
    printf("Num buffers: %d\n", root->Node.WaitFree.buffers);
    printf("Type: %s\n", root->Node.WaitFree.type);
    printf("----------- WaitFree ------------\n");
    printTree(root->Node.WaitFree.Next);
    break;

  case kSignal:
    printf("----------- Signal ------------\n");
    printf("Name: %s\n", root->Node.Signal.name);
    printTree(root->Node.Signal.user);
    printf("----------- Signal ------------\n");
    printTree(root->Node.Signal.Next);
    break;

  case kSemaphore:
    printf("----------- Semaphore ------------\n");
    printf("Name: %s\n", root->Node.Semaphore.name);
    printTree(root->Node.Semaphore.user);
    printf("Ceil: %d\n", root->Node.Semaphore.ceil);
    printf("----------- Semaphore ------------\n");
    printTree(root->Node.Semaphore.Next);
    break;

  case kUser:
    printf("User: %s\n",root->Node.User.string);
    printf("Prio: %d\n",root->Node.User.prio);
    printTree(root->Node.User.Next);
    break;

  case kIntConst:
    printf("%d\n",root->Node.IntConst.Value);
    break;

  case kStringConst:
    printf("%s\n",root->Node.StringConst.Value);
    break;

  case kTasks:
    printTree(root->Node.Tasks.hard_tasks);
    printTree(root->Node.Tasks.soft_tasks);
    break;

  case kResources:
    printTree(root->Node.Resources.communications);
    printTree(root->Node.Resources.synchronizations);
    break;

  case kCommunications:
    printTree(root->Node.Communications.wait_free);
    break;

  case kSynchronizations:
    printTree(root->Node.Synchronizations.signals);
    printTree(root->Node.Synchronizations.semaphores);
    break;

  default:
    perror("Warning! Syntax tree handler:\n");
    perror("  An undefined node was found.\n");
  }
}

void printNode(tTree root)
{
  if(root==NULL)
    return;

  switch(root->Kind){
  case kProgram:
    printf("SystemMode: %d\n", root->Node.Program.systemMode);
    printf("Ram: %d\n", root->Node.Program.ram);
    printTree(root->Node.Program.mode);
    break;

  case kMode:
    printf("----------- Mode ------------\n");
    printf("Name: %s\n", root->Node.Mode.name);
    printf("Clock: %d\n", root->Node.Mode.clock);
    printTree(root->Node.Mode.tasks);
    printTree(root->Node.Mode.resources);
    printf("----------- Mode ------------\n");
    break;

  case kTask:
    printf("Task: \n");
    printf("----------- Task ------------\n");
    printf("Name: %s\n", root->Node.Task.name);
    printf("Activator: ");
    printTree(root->Node.Task.activator);
    printf("Offset: %d\n", root->Node.Task.offset);
    printf("Deadline: %d\n", root->Node.Task.deadline);
    printf("Priority: %d\n", root->Node.Task.priority);
    printf("Stack size: %d\n", root->Node.Task.stack);
    printf("Routine: %s\n", root->Node.Task.routine);
    printf("Arguments: ");
    printTree(root->Node.Task.argument);
    printf("err_routine: \n");
    printNode(root->Node.Task.err_routine);
    printf("----------- Task ------------\n");
    break;

  case kWaitFree:
    printf("----------- WaitFree ------------\n");
    printf("Name: %s\n", root->Node.WaitFree.name);
    printf("Writer: %s\n", root->Node.WaitFree.writer);
    printTree(root->Node.WaitFree.reader);
    printf("Num buffers: %d\n", root->Node.WaitFree.buffers);
    printf("Type: %s\n", root->Node.WaitFree.type);
    printf("----------- WaitFree ------------\n");
    break;

  case kSignal:
    printf("----------- Signal ------------\n");
    printf("Name: %s\n", root->Node.Signal.name);
    printTree(root->Node.Signal.user);
    printf("----------- Signal ------------\n");
    break;

  case kSemaphore:
    printf("----------- Semaphore ------------\n");
    printf("Name: %s\n", root->Node.Semaphore.name);
    printTree(root->Node.Semaphore.user);
    printf("Ceil: %d\n", root->Node.Semaphore.ceil);
    printf("----------- Semaphore ------------\n");
    break;

  case kUser:
    printf("User: %s\n",root->Node.User.string);
    printf("Prio: %d\n",root->Node.User.prio);
    break;

  case kIntConst:
    printf("%d\n",root->Node.IntConst.Value);
    break;

  case kStringConst:
    printf("%s\n",root->Node.StringConst.Value);
    break;

  case kTasks:
    printTree(root->Node.Tasks.hard_tasks);
    printTree(root->Node.Tasks.soft_tasks);
    break;

  case kResources:
    printTree(root->Node.Resources.communications);
    printTree(root->Node.Resources.synchronizations);
    break;

  case kCommunications:
    printTree(root->Node.Communications.wait_free);
    break;

  case kSynchronizations:
    printTree(root->Node.Synchronizations.signals);
    printTree(root->Node.Synchronizations.semaphores);
    break;

  default:
    perror("Warning! Syntax tree handler:\n");
    perror("  An undefined node was found.\n");
  }
}


/*
  Name:   allocateNode
  Input:  int pKind, kind of node,
          int pLineNr
  Output: tTree, a pointer to the allocated node
  Calls:  -
  Descr:  Allocates space for a treeNode and
          initiates it.
*/
tTree allocateNode (int pKind, int pLineNr)
{
  tTree temp = (tTree)calloc (1, sizeof (struct treeNode));

  if (temp != NULL) {
    temp->Kind = pKind;
    temp->LineNr = pLineNr;
  }
  else {
    fprintf (stderr, "Couldn't alllocate a tree node of type: %d\n\r", pKind);
    exit (EXIT_FAILURE);
  }
  return (temp);
}

/*
  Name:   mProgram
  Input:  int sys_mode, system mode
          int ram, RAM of the system
    tTree mode, a list of modes
    int pLineNr
  Output: tTree, a pointer to a kProgram treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kProgram node
*/
tTree mProgram(int sys_mode, int ram, tTree mode, int pLineNr)
{
  tTree temp = allocateNode (kProgram, pLineNr);

  temp->Node.Program.systemMode = sys_mode;
  temp->Node.Program.ram = ram;
  temp->Node.Program.mode = mode;

  return temp;
}

/*
  Name:   mMode
  Input:  char *name, id of the mode
          int clock, clock resolution in the mode
    tTree tasks, the tasks branch of the mode
    tTree resources, the resources branch of the mode
    int pLineNr
  Output: tTree, a pointer to a kMode treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kMode node
*/
tTree mMode (char  *name,
             int   clock,
             tTree tasks,
             tTree resources,
             int   pLineNr)
{
  tTree temp = allocateNode (kMode, pLineNr);
  strcpy(temp->Node.Mode.name,name);
  temp->Node.Mode.clock = clock;
  temp->Node.Mode.tasks = tasks;
  temp->Node.Mode.resources = resources;
  temp->Node.Mode.Next=NULL;
  return temp;
}


/*
  Name:   mTask
  Input:  char *name, id of the task
          tTree activator, activator branch of the task
    int offset,
    int deadline,
    int priority,
    int stack, stack size,
    char *routine, start address,
    tTree arguments, argument string,
    tTree err_routine, error handler address
    int pLineNr
  Output: tTree, a pointer to a kTask treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kTask node
*/
tTree mTask (int   type,
       char  *name,
       int   period,
       int   min_interarrival_time,
       int   max_interarrival_time,
             tTree activator,
             int   offset,
             int   deadline,
             int   priority,
       int   stack,
             char  *routine,
             tTree argument,
             tTree err_routine,
             int   pLineNr)
{
  tTree temp = allocateNode (kTask, pLineNr);
  temp->Node.Task.type = type;
  strcpy(temp->Node.Task.name, name);
  temp->Node.Task.period = period;
  temp->Node.Task.min_interarrival_time = min_interarrival_time;
  temp->Node.Task.max_interarrival_time = max_interarrival_time;
  temp->Node.Task.activator = activator;
  temp->Node.Task.offset = offset;
  temp->Node.Task.deadline = deadline;
  temp->Node.Task.priority = priority;
  temp->Node.Task.stack = stack;
  strcpy(temp->Node.Task.routine, routine);
  temp->Node.Task.argument = argument;
  temp->Node.Task.err_routine = err_routine;
  temp->Node.Task.Next=NULL;
  return temp;
}

/*
  Name:   mWaitFree
  Input:  char *name, id of the waitfree channel
          char *writer, id of the writer
    tTree reader, a list of readers
    int num_buf, number of buffers
    char *type, message type
    int pLineNr
  Output: tTree, a pointer to a kWaitFree treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kWaitFree node
*/
tTree mWaitFree (char *name, char *writer, tTree reader,
     int num_buf, char *type, int pLineNr)
{
  tTree temp = allocateNode (kWaitFree, pLineNr);
  strcpy(temp->Node.WaitFree.name, name);
  strcpy(temp->Node.WaitFree.writer, writer);
  temp->Node.WaitFree.reader = reader;
  temp->Node.WaitFree.buffers = num_buf;
  strcpy(temp->Node.WaitFree.type, type);
  temp->Node.WaitFree.Next=NULL;
  return temp;
}


/*
  Name:   mSignal
  Input:  char *name, id of the signal
          tTree users, a list of the users
    int pLineNr
  Output: tTree, a pointer to a kSignal treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kSignal node
*/
tTree mSignal (char *name, tTree users, int pLineNr)
{
  tTree temp = allocateNode (kSignal, pLineNr);
  strcpy(temp->Node.Signal.name, name);
  temp->Node.Signal.user = users;
  temp->Node.Signal.Next=NULL;
  return temp;
}

/*
  Name:   mSemaphore
  Input:  char *name, id of the semaphore
          tTree users, a list of the users
    int pLineNr
  Output: tTree, a pointer to a kSemaphore treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kSemaphore node
*/
tTree mSemaphore (char *name, tTree users, int pLineNr)
{
  tTree temp = allocateNode (kSemaphore, pLineNr);
  strcpy(temp->Node.Semaphore.name, name);
  temp->Node.Semaphore.user = users;
  temp->Node.Semaphore.ceil = -1;
  temp->Node.Semaphore.Next=NULL;
  return temp;
}


/*
  Name:   mUser
  Input:  char *string, id of the user task
    int pLineNr
  Output: tTree, a pointer to a kUser treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kUser node
*/
tTree mUser (char *string, int pLineNr)
{
  tTree temp = allocateNode (kUser, pLineNr);
  strcpy(temp->Node.User.string, string);
  temp->Node.User.prio = -1;
  temp->Node.User.Next=NULL;
  return temp;
}


/*
  Name:   mIntConst
  Input:  int pValue, int value
    int pLineNr
  Output: tTree, a pointer to a kIntConst treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kIntConst node
*/
tTree mIntConst (int pValue, int pLineNr)
{
  tTree temp = allocateNode (kIntConst, pLineNr);
  temp->Node.IntConst.Value = pValue;
  return temp;
}

/*
  Name:   mStringConst
  Input:  char *pValue, string value
    int pLineNr
  Output: tTree, a pointer to a kStringConst treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kStringConst node
*/
tTree mStringConst(char *pValue, int pLineNr)
{
  tTree temp = allocateNode (kStringConst, pLineNr);
  strcpy (temp->Node.StringConst.Value, pValue);
  return temp;
}

/* Some abstract node make-functions */

/*
  Name:   mTasks
  Input:  tTree hard_tasks, hard tasks of the mode
          tTree soft_tasks, soft tasks of the mode
    int pLineNr
  Output: tTree, a pointer to a kTasks treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kTasks node
*/
tTree mTasks(tTree hard_tasks, tTree soft_tasks, int pLineNr)
{
  tTree temp = allocateNode (kTasks, pLineNr);
  temp->Node.Tasks.hard_tasks = hard_tasks;
  temp->Node.Tasks.soft_tasks = soft_tasks;
  return temp;
}

/*
  Name:   mResources
  Input:  tTree communications, comm channels of the mode
          tTree synchronizations, synch resources
    int pLineNr
  Output: tTree, a pointer to a kResources treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kResources node
*/
tTree mResources(tTree communications, tTree synchronizations, int pLineNr)
{
  tTree temp = allocateNode (kResources, pLineNr);
  temp->Node.Resources.communications = communications;
  temp->Node.Resources.synchronizations = synchronizations;
  return temp;

}

/*
  Name:   mCommunications
  Input:  tTree wait_free, list of waitfree channel nodes
    int pLineNr
  Output: tTree, a pointer to a kCommunications treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kCommunications node
*/
tTree mCommunications(tTree wait_free, int pLineNr)
{
  tTree temp = allocateNode (kCommunications, pLineNr);
  temp->Node.Communications.wait_free = wait_free;
  return temp;
}


/*
  Name:   mSynchronizations
  Input:  tTree signals, a list of signals
          tTree sempahores, a list of semaphores
    int pLineNr
  Output: tTree, a pointer to a kSemaphores treeNode node
  Calls:  allocateNode
  Descr:  Allocates and initiates for a kSemaphores node
*/
tTree mSynchronizations(tTree signals, tTree semaphores, int pLineNr)
{
  tTree temp = allocateNode (kSynchronizations, pLineNr);
  temp->Node.Synchronizations.signals = signals;
  temp->Node.Synchronizations.semaphores = semaphores;
  return temp;
}


/*
  Name:   freeTree
  Input:  tTree root, a syntax tree
  Output: -
  Calls:  freeTree, recursively
  Descr:  Free the syntax tree
*/
void freeTree(tTree root)
{
  if(root==NULL)
    return;

  switch(root->Kind){
  case kProgram:
    freeTree(root->Node.Program.mode);
    free(root);
    break;

  case kMode:
    freeTree(root->Node.Mode.tasks);
    freeTree(root->Node.Mode.resources);
    freeTree(root->Node.Mode.Next);
    free(root);
    break;

  case kTask:
    freeTree(root->Node.Task.activator);
    freeTree(root->Node.Task.argument);
    freeTree(root->Node.Task.err_routine);
    freeTree(root->Node.Task.Next);
    free(root);
    break;

  case kWaitFree:
    freeTree(root->Node.WaitFree.reader);
    freeTree(root->Node.WaitFree.Next);
    free(root);
    break;

  case kSignal:
    freeTree(root->Node.Signal.user);
    freeTree(root->Node.Signal.Next);
    free(root);
    break;

  case kSemaphore:
    freeTree(root->Node.Semaphore.user);
    freeTree(root->Node.Semaphore.Next);
    free(root);
    break;

  case kUser:
    freeTree(root->Node.User.Next);
    free(root);
    break;

  case kIntConst:
    free(root);
    break;

  case kStringConst:
    free(root);
    break;

  case kTasks:
    freeTree(root->Node.Tasks.hard_tasks);
    freeTree(root->Node.Tasks.soft_tasks);
    break;

  case kResources:
    freeTree(root->Node.Resources.communications);
    freeTree(root->Node.Resources.synchronizations);
    break;

  case kCommunications:
    freeTree(root->Node.Communications.wait_free);
    break;

  case kSynchronizations:
    freeTree(root->Node.Synchronizations.signals);
    freeTree(root->Node.Synchronizations.semaphores);
    break;

  default:
    perror("Warning! Leeking Memory\n");
    perror("When freeing nodes in the syntax tree an undefined node was found.\n");
  }
}
