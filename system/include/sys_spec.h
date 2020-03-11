

#ifdef IARH8300

  #include <inh8300.h>  // allow intrinsic functions H8/300 and IAR stuff, 
                        // e.g., the C_task declarations which avoids register 
                        // storage on the stack 


  /* Do not do anything about C_task, C_task is defined by IAR Embedded Workbench */
#else
  #define C_task
#endif