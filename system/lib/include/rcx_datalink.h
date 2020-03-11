/*  \file   datalink.h
 *	 \brief  A hard ware dependent libary for the Lego Mindstorm RCX.
 *          Developed for the Asterix RTOS project.
 *  \date   3 Maj 2000
 *	 \author Anders Davidsson, Joakim Lindgren.
 *
 *
 *   For more information or contact please visit:
 *   http://www.mds.mdh.se/~dtv96adn/asterix/
 */

#ifndef __DATALINK__
#define __DATALINK__

#define PACKAGE_SIZE 65536 			/* max package size */

#define FRAME_SIZE 39
#define DATA_SIZE 16       	/* max real data in a single frame */
#define ACK 0x06
#define NAK 0x15

/* some primitives to the communication protocol */

/*
Name:		packageSend
Input:	char *data, a pointer to the data to send
			unsigned int dsize, number of bytes to send, max 65536 bytes.
Output:	-
Calls: 	DLSend
Descr: 	This function devides the package into the max size for frames
*/
void packageSend(char *data, unsigned short dsize);

/*
Name:		memSend
Input:   char *data, a pointer to data to send.
	 unsigned int dsize, the size of the data to send. Max 64kb
Output:  -
Calls:   packageSend
Descr:	This is a small application that send a piece of memory to the PC that when dumps is to a file.
			This function sends a package with the memory dump. To do this we first sends the address in
			one package and then the memory data. A corresponding receiving routine must of course be runing on the PC.
			(-called memDump)
*/
void memSend(char *data, unsigned short dsize);


#endif






