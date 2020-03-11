

#define TRUE 1
#define FALSE 0


extern int type_error;

extern void check_signal_arg(tTree program);
extern void check_task_arg(tTree program);
extern void check_semaphores(tTree program);
extern void make_unique_prio(tTree program);
extern void check_wait_free(tTree program);
