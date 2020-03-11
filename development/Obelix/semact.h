/*
   File:     semact.h
   Autor:    Anders Davidsson, Joakim Lindgren
   Revision: 00/03/31 Reviewed
   Desc:     A simple package of semantic controlls of a
             Obelix configuration.
*/


#define TRUE 1
#define FALSE 0
#define MIN_STACK_SIZE 10

/*extern int type_error;*/

void check_signal_arg(tTree program);
void check_task_arg(tTree program);
void check_wait_free(tTree program);
void make_unique_prio(tTree program);
void check_semaphores(tTree program);
void checkStacks(tTree program);
