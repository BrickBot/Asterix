/**************************************************************
    ##################################################
        ASTERIX RTOS
        COPYRIGHT (c) 2000, 2001
    ##################################################

  Hardware Adaption Layer (HAL)
  Hitatchi H8/300

  Timer interrupt related functions that are target cpu specific

  File:     hal_timer.s20
  Date:     2001-03-13
  Authors:  Henrik Thane, Daniel Sundmark,
            Anders Petterson, Andreas Engberg

**************************************************************/

/***************************************************************
  Externals
 **************************************************************/
        .EXTERN     _timerVal

/***************************************************************
  Function prototypes
 **************************************************************/
        .GLOBAL     _hal_startTimer
        .GLOBAL     _hal_stopTimer
        .GLOBAL     _hal_readTimer
        .GLOBAL     _hal_getFRCval
        .GLOBAL     _hal_initTimer

/***************************************************************
  Defines
 **************************************************************/


/**************************************************************
  The COMMON_TIMER module
**************************************************************/
        .NAME       COMMON_TIMER


; /* Tell the assembler that this belong to the code segment */
        .section    .text

/**************************************************************
  void hal_startTimer(void)

  Start timertick.
**************************************************************/
_hal_startTimer:
              mov.w   #0xff90,r0      // Enable compare A interrupt
              bset.b  #3,@r0          // Set bit 3 in TIER
              andc    #0x7f,ccr       // Enable interrupt
              rts

/**************************************************************
  void hal_stopTimer(void)

  Stop timertick.
**************************************************************/
_hal_stopTimer:
              orc     #0x80,ccr
              mov.w   #0xff90,r0      ; Enable compare A interrupt
              bclr.b  #3,@r0          ; Set bit 3 in TIER      
              rts

/**************************************************************
  uint16   hal_readTimer(void);

  Read the free running counter 
**************************************************************/
_hal_readTimer:
              mov.w   #0xff92,r1      ; Read timer
              mov.w   @r1,r6          ; Return value
              rts
              

/**************************************************************
  ticks_t hal_getFRCval(void)


  Usage       : hal_getFRCval();
  Arguments   : none
  Returns     : the value of the Free Running Counter

  Purpose     : Read system-timer

**************************************************************/
_hal_getFRCval:
              mov.w   @0xff92,r6
              rts
           
/**************************************************************
  void initTimer(void (*timerfunc)(void))

  Read the free running counter 
**************************************************************/
_hal_initTimer:
              orc     #0x80,ccr       ; Disable interrupts
              push    r0              ; The timerfunction
      
              mov.w   #0xff91,r0      ; Clear counter on compare match
              mov.b   #0x01,r1l
              mov.b   r1l,@r0         ; Set bit 0 in TCSR
      
              mov.w   #0xff96,r0      ; Scale 32
              mov.b   #0x02,r1l
              mov.b   r1l,@r0         ; Set bit 1 in TCR
                      
      
              mov.w   #0xff94,r0      ; Compare A register
              mov.w   @_timerVal,r1
              mov.w   r1,@r0          ; Count up to chosenvalue
      
              mov.w   #0xfda2,r0      ; ORCA Interrupt vector
              pop     r1              ; Put timerfunc in r1
              mov.w   r1,@r0          ; Set timerrut in vector      
              rts

              .END
