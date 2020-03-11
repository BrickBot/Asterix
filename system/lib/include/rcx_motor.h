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


#ifndef __MOTOR__
#define __MOTOR__

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 2 
#define MOTOR_STOP 3 
#define MOTOR_FLOAT 4

#define MOTOR_A  0x2000
#define MOTOR_B  0x2001
#define MOTOR_C  0x2002

#define MOTOR_CONTROL 0xf000

/*
	Name:		setMotor
	Input:   	short code, which motor to affect
			short dir, what do do with the motor
	Output: 	-
	Calls:	-
	Descr:  	sets the staus to the motor, runing, float, stop...
*/
void setMotor(short code, short dir);

#endif
