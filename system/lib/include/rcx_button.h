/*  \file   button.h
 *   \brief  A hard ware dependent libary for the Lego Mindstorm RCX.
 *          Developed for the Asterix RTOS project.
 *  \date   3 Maj 2000
 *   \author Anders Davidsson, Joakim Lindgren.
 *
 *
 *   For more information or contact please visit:
 *   http://www.mds.mdh.se/~dtv96adn/asterix/
 */

#ifndef __BUTTON__
#define __BUTTON__

#include <rcx_undef.h>

#define VIEW_BUTTON 2
#define PRGM_BUTTON 4
#define RUN_BUTTON  1
#define NON_BUTTON  0


/*
 * Read buttons (short *ptr)
 *
 *   Set *ptr = 0
 *   If port 7 bit 6 (@ffbe & 40) is set, *ptr |= 02 (view button pressed)
 *   If port 7 bit 7 (@ffbe & 80) is set, *ptr |= 04 (prgm button pressed)
 *   If port 4 bit 2 (@ffb7 & 04) is set, *ptr |= 01 (run button pressed)
 */

static inline void
readButtons (short *ptr)
{
     __rcall2 (0x1fb6, 0x3000, (short)ptr);
}


#endif

