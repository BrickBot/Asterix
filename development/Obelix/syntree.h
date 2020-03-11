/*
    File:     syntree.h
    Author:   Joakim Lindgren, Anders Davidsson
    Revision: 00/03/30
    Descr:    A package support for creating syntax tree based on
              the Obelix configuration grammar.
*/


#define MAX_STRING_LENGTH 256
#define ID_MAX_SIZE 256

# define NO_NODE (tTree) 0L
# define SYS_DEFINED -1

/* Following types describe which kind of nodes that exists */
# define kProgram           1
# define kMode              2
# define kClock             3
# define kTask              4
# define kWaitFree          5
# define kSignal            6
# define kSemaphore         7
# define kUser              8
# define kIntConst          9
# define kStringConst       10

# define kTasks             11
# define kResources         12
# define kCommunications    13
# define kSynchronizations  14
# define kPeriodic          18
# define kAperiodic         19
# define kSporadic          20

typedef struct treeNode *tTree;

/* Node type defenitions */
typedef struct _yProgram{
  int systemMode;
  int ram;
  tTree mode;
}yProgram;

typedef struct _yMode{
  tTree Next;
  char  name[MAX_STRING_LENGTH];
  int   clock;
  tTree tasks;
  tTree resources;
}yMode;

typedef struct _yTask{
  tTree Next;
  int   type;
  int   period;
  int   min_interarrival_time;
  int   max_interarrival_time;
  char  name[MAX_STRING_LENGTH];
  tTree activator;
  int   offset;
  int   deadline;
  int   priority;
  int   stack;
  char  routine[MAX_STRING_LENGTH];
  tTree  argument;
  tTree  err_routine;
  unsigned int semaphore_flag;
}yTask;

typedef struct _yWaitFree{
  tTree Next;
  char  name[MAX_STRING_LENGTH];
  char  writer[MAX_STRING_LENGTH];
  tTree reader;
  int   buffers;
  char  type[MAX_STRING_LENGTH];
} yWaitFree;

typedef struct _ySignal{
  tTree Next;
  char  name[MAX_STRING_LENGTH];
  tTree user;
} ySignal;

typedef struct _ySemaphore{
  tTree Next;
  char  name[MAX_STRING_LENGTH];
  tTree user;
   int  ceil;
} ySemaphore;

typedef struct _yUser{
  char  string[MAX_STRING_LENGTH];
  int   prio;
  tTree Next;
} yUser;


/* Now some basic types  */
typedef struct _yIntConst{
  int   Value;
} yIntConst;

typedef struct _yStringConst{
  char  Value[MAX_STRING_LENGTH];
} yStringConst;


/* Some abstract nodes to group the other ones */
typedef struct _yTasks{
  tTree hard_tasks;
  tTree soft_tasks;
}yTasks;

typedef struct _yResources{
  tTree communications;
  tTree synchronizations;
}yResources;

typedef struct _yCommunications{
  tTree wait_free;
}yCommunications;

typedef struct _ySynchronizations{
  tTree signals;
  tTree semaphores;
}ySynchronizations;



/* The basic node type in the tree */
struct treeNode{
  int   Kind;
  int   LineNr;

  union {
    yProgram Program;
    yMode Mode;
    yTask Task;
    yWaitFree WaitFree;
    ySignal Signal;
    ySemaphore Semaphore;
    yUser User;
    yIntConst IntConst;
    yStringConst StringConst;
    yTasks Tasks;
    yResources Resources;
    yCommunications Communications;
    ySynchronizations Synchronizations;
  }Node;

};



/* Functions to build a abstract syntax tree */

tTree mProgram(int sys_mode, int ram, tTree mode, int pLineNr);

tTree mMode (char  *name,
             int    clock,
             tTree tasks,
             tTree resources,
             int pLineNr);

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
             int   pLineNr);


tTree mWaitFree (char *name, char *writer, tTree reader, int num_buf, char *type, int pLineNr);


tTree mSignal (char *name, tTree users, int pLineNr);
tTree mSemaphore (char *name, tTree users, int pLineNr);
tTree mUser (char *string, int pLineNr);
tTree mIntConst (int pValue, int pLineNr);
tTree mStringConst(char *pValue, int pLineNr);

tTree mTasks(tTree hard_tasks, tTree soft_tasks, int pLineNr);
tTree mResources(tTree communications, tTree synchronizations, int pLineNr);
tTree mCommunications(tTree wait_free, int pLineNr);
tTree mSynchronizations(tTree signals, tTree semaphores, int pLineNr);

tTree connectNode (tTree pUsers, tTree pUser, int Kind);

/* debugging functions */
void printTree(tTree root);
void printNode(tTree node);
void printTaskPrio(tTree task);
void printSemaphoreCeil(tTree root);

/* Clean up */
void freeTree(tTree root);



