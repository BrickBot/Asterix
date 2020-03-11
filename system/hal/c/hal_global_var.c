/*
    ##################################################
        ASTERIX RTOS                                
        COPYRIGHT (c) 2000, 2001                    
    ##################################################
*/
#include <hal_kertypes.h>

cpureg_t        register0_temp;
cpureg_t        register6_temp;
programcounter_t  programcounter_temp;

cpureg_t        tmpregs[7];
cpureg_t*       tmpregAdr = &tmpregs[0];
