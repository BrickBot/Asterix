/*  \file   rcx_lib.c
 *	 \brief  A hard ware dependent libary for the Lego Mindstorm RCX.
 *          Developed for the Asterix RTOS project.
 *  \date   3 Maj 2000
 *	 \author Anders Davidsson, Joakim Lindgren.
 *
 *
 *   For more information or contact please visit:
 *   http://www.mds.mdh.se/~dtv96adn/asterix/
 */

#ifndef __UNDEF__
#define __UNDEF__


extern char rcall0 (short a);
extern char rcall1 (short a, short p0);
extern char rcall2 (short a, short p0, short p1);
extern char rcall3 (short a, short p0, short p1, short p2);
extern char rcall4 (short a, short p0, short p1, short p2, short p3);

/*
	Name: 	mDelay
	Input:	int mtime, number of millisecond to delay
	Output: 	-
	Calls: 	-
	Descr:	This routine takes mtime milisecond to execute.
				Note: It is NOT any kind of system call
*/
extern void mDelay(int time);

extern void reset(void);
#endif
