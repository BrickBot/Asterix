/**************************************************************
    ##################################################
        ASTERIX RTOS                                
        COPYRIGHT (c) 2000, 2001                    
    ##################################################

    File: semaphore.c

    Functions for handling of semaphores. 
    Supports the Immediate Inheritance Protocol 

    Date:     2001-03-25
    Authors:  Anders Petterson, Henrik Thane, 
              Andreas Engberg

**************************************************************/
#include <os_kernel.h>
#ifdef	__SEMAPHORE__

#ifndef __TIME_MEASURE__

/***************************************************************
    void releaseSemaphore( semid_t semid )  
 
    Arguments:  The semaphore identifier
    Returns:    Zip
    Purpose:    Let another tasks have the possibility to claim 
                the semaphore.
    Uses:        yield();
***************************************************************/
void releaseSemaphore( semid_t semid )
{
    uint16            nrsem_inlist                    = 0 ;
    uint16            semaphore_removed               = 0;
    uint16            lowerpriority_semaphore_remain  = 0;
    semaphorelist_t*  exec_semaphorelist;
        
    /*-----------------------------------------------------------
    Assumptions: We assume that the semaphores that the tasks 
    can use are stored as references in an array in order of 
    increasing priority (priority ceiling). The first element in 
    the array states the number of semaphores in the list. The 
    semaphore references are identifiers which are shifted 1bit
    up, this to make room for the least significant bit which
    indicate if the semaphore is taken/wanted or not.
    -----------------------------------------------------------*/

    /*-----------------------------------------------------------
    Get the list of semaphores that the currently executing task
    can use, as well as how many semaphores there are in the list.  
    -----------------------------------------------------------*/
    exec_semaphorelist = TCBList[ExecTask].semaphorelist;
    nrsem_inlist       = exec_semaphorelist[0];
    
    /*-----------------------------------------------------------
    Check if the semaphore to be released, semid, is in the list 
    of valid semaphores for the currently executing task. We 
    begin with the highest priority semaphore (the last entry in
    the list)
    -----------------------------------------------------------*/
    for( ;nrsem_inlist > 0 ; nrsem_inlist-- )
    {
        /*-----------------------------------------------------------
        Is the semaphore in the list? The least significant bit of 
        the semaphore identifiers indicate if the semaphore is taken.
        -----------------------------------------------------------*/
        if( semid == (exec_semaphorelist[nrsem_inlist] >> 1)  )
        {
            exec_semaphorelist[nrsem_inlist]  = (semid << 1);
            semaphore_removed                 = 1;
        } 
        else 
        {
            if( semaphore_removed && !lowerpriority_semaphore_remain )
            {
                if( (exec_semaphorelist[nrsem_inlist] & 0x1) )
                {
                    /*-----------------------------------------------------------
                    We have released a higher priority semaphore and found another
                    semaphore which is taken/wanted. Change the curent priority
                    of the executing task to the priority ceiling of the just 
                    found lower priority semaphore.
                    -----------------------------------------------------------*/
                    displist[ExecTask].current_priority = 
                        semaphore[ (exec_semaphorelist[nrsem_inlist] >> 1) ].priority_ceiling;
                    lowerpriority_semaphore_remain      = 1;
                }
            }
        }
    }

    /*-----------------------------------------------------------
    We have released a higher priority semaphore but found no 
    other that was currently taken/wanted. Return the priority of
    the executing task to its original priority. 
    -----------------------------------------------------------*/
    if( semaphore_removed && !lowerpriority_semaphore_remain )
        displist[ExecTask].current_priority = displist[ExecTask].original_priority;


    /*-----------------------------------------------------------
    After releasing this higher priority semaphore check if there 
    are higher priority tasks in the ready queue. 
    -----------------------------------------------------------*/
    if( semaphore_removed )
    {    
        hal_yield();        
    }
    else 
    {
    /*-----------------------------------------------------------
    Illegal access to semaphore, although harmeless.
    -----------------------------------------------------------*/
    }
}
#else // __TIME_MEASURE__
void releaseSemaphore( semid_t semid )
{
}
#endif

#ifndef __TIME_MEASURE__
/***************************************************************
    void getSemaphore( semid_t semid )
 
    Arguments:  The semaphore identifier
    Returns:    Zip
    Purpose:    Lets the calling task claim the semaphore. If 
                taken, the task's priority is elevated to the 
                semaphore's priority ceiling if it is higer than
                the task's current priority
***************************************************************/
void getSemaphore( semid_t semid )
{
    uint16                nrsem_inlist,
                          found = 0;
    semaphorelist_t*      exec_semaphorelist;

    /*-----------------------------------------------------------
    Assumptions: We assume that the semaphores that the tasks 
    can use are stored as references in an array in order of 
    increasing priority (priority ceiling). The first element in 
    the array states the number of semaphores in the list. The 
    semaphore references are identifiers which are shifted 1bit
    up, this to make room for the least significant bit which
    indicate if the semaphore is taken/wanted or not.
    -----------------------------------------------------------*/

    
    /*-----------------------------------------------------------
    Get the list of semaphores that the currently executing task
    can use, as well as how many semaphores there are in the list.  
    -----------------------------------------------------------*/
    exec_semaphorelist = TCBList[ExecTask].semaphorelist;
    nrsem_inlist       = exec_semaphorelist[0];
    
    /*-----------------------------------------------------------
    Check if the wanted semaphore, semid, is in the list of valid 
    semaphores. We begin with the highest priority semaphore (the 
    last entry in the list)
    -----------------------------------------------------------*/
    for(; nrsem_inlist > 0 ; nrsem_inlist-- )
    {
        /*-----------------------------------------------------------
        Is the semaphore in the list? The least significant bit of 
        the semaphores in the list indicate if the semaphore is 
        already taken.
        -----------------------------------------------------------*/
        if( semid == (exec_semaphorelist[nrsem_inlist] >> 1)  )
        {
            /*-----------------------------------------------------------
            Found it! Assign it as taken; shift the semaphore id up one
            bit and then set the LSB to 1.
            -----------------------------------------------------------*/
            exec_semaphorelist[nrsem_inlist]  = (semid << 1) + 1;
            found = 1;
    
            /*-----------------------------------------------------------
            If the priority ceiling of the just taken semaphore is 
            greater than the current priority for the executing task, 
            then change the current priority to the semaphore's ceiling. 
            -----------------------------------------------------------*/
            if( semaphore[semid].priority_ceiling > displist[ExecTask].current_priority )
            {
                displist[ExecTask].current_priority = semaphore[semid].priority_ceiling;
            }
        } 
        else 
        {
        /* Add jitter minimization here */
        }
    }
    
    /*-----------------------------------------------------------
    Check if we did not find, semid, among the valid semaphores 
    for the currently executing tasks. If so, this is an illegal 
    access to the semaphore. 
    -----------------------------------------------------------*/
    if( !found )
        TCBList[ExecTask].errorHandler(ERROR_ILLEGAL_SEMAPHORE_ACCESS);
}
#else //__TIME_MEASURE__

void getSemaphore(semid_t semid)
{
}
#endif


#endif	/* __SEMAPHORE__ */
