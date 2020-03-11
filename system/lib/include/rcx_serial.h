
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

/* some port definitions */
/*! port 4 data direction register */
#ifndef __SERIAL__
#define __SERIAL__

extern  			 unsigned char PORT4_DDR;

//! port 4 I/O register
extern volatile unsigned char PORT4;

//! port 5 data direction register
extern          unsigned char PORT5_DDR;

//! port 5 I/O register
extern volatile unsigned char PORT5;

//! port 6 I/O register
extern volatile unsigned char PORT6;

//! port 6 I/O register
extern volatile unsigned char PORT6_DDR;

//! serial receive data register
extern volatile unsigned char S_RDR;

//! serial transmit data register
extern          unsigned char S_TDR;

//! serial mode register
extern          unsigned char S_MR;

//! serial control register
extern          unsigned char S_CR;

//! serial status register
extern volatile unsigned char S_SR;

//! serial baud rate register
extern          unsigned char S_BRR;

//! serial / timer control register
extern          unsigned char S_TCR;

//
// serial communication register bitmasks
//

// IR carrier stuff
//! timer 1 control register
extern          unsigned char T1_CR;

//! timer 1 control / status register
extern volatile unsigned char T1_CSR;

//! timer 1 constant A register
extern          unsigned char T1_CORA;

//! timer 1 constant B register
extern          unsigned char T1_CORB;

//! timer 1 counter register
extern volatile unsigned char T1_CNT;


#define SMR_SYNC	0x80	   // in sync mode, the other settings
#define SMR_ASYNC	0x00	   // have no effect.
#define SMR_7BIT	0x40
#define SMR_8BIT	0x00
#define SMR_P_NONE	0x00
#define SMR_P_EVEN	0x20
#define SMR_P_ODD	0x30
#define SMR_1STOP	0x00
#define SMR_2STOP	0x08
#define SMR_MP		0x04	   // multiprocessing -> no parity
					// for mp, mpe in STRC must be set
#define SMR_CLOCK	0x00	   // clock rate for baud rate generator
#define SMR_CLOCK_4	0x01	   // pclock / 4
#define SMR_CLOCK_16	0x02	   // pclock / 16
#define SMR_CLOCK_64	0x03	   // pclock / 64


#define SCR_TX_IRQ	0x80	   // TIE transmit irq enable
#define SCR_RX_IRQ	0x40	   // RIE receive / recv err irq enable
#define SCR_TRANSMIT	0x20	   // enable transmission
#define SCR_RECEIVE	0x10	   // enable receiving
#define SCR_MP_IRQ	0x08	   // multiprocessing receive irq
#define SCR_TE_IRQ	0x04	   // TEI transmit end irq enable
#define SCR_INT_CLOCK	0x00	   // internal clock source
#define SCR_EXT_CLOCK	0x02	   // external clock source
#define SCR_CLOCK_OUT	0x01	   // output internal clock to SCK pin


#define SSR_TRANS_EMPTY	0x80	   // transmit buffer empty
#define SSR_RECV_FULL	0x40	   // receive buffer full
#define SSR_OVERRUN_ERR	0x20	   // overrun error
#define SSR_FRAMING_ERR	0x10	   // framing error
#define SSR_PARITY_ERR	0x08	   // parity error
#define SSR_ERRORS      0x38       // all errors
#define SSR_TRANS_END	0x04	   // transmission end because buffer empty
#define SSR_MP		0x02	   // multiprocessor
#define SSR_MP_TRANSFER	0x01	   // multiprocessor bit transfer




void serialInit(void);
void serialShutdown(void);

#define BLOCKED 0
#define NONBLOCKED 1


/*
	Name: 	readSerial
	Input:	char *buf, a buffert to to received data.
				int length, number og bytes to read.
				int block, 1; blocking read of length bytes, 0; nonblocking read.
	Output: 	int, number of bytes read. In blocking mode, 1 byte is probably max.
	Calls:	serialError
	Descr: 	BLOCK-read of length bytes.
		NON-BLOCK-read of data, tries to read 1 byte and return 1 if a
		byte was read.
*/
int readSerial(char *buf, int length, int block);

/*
	Name: 	writeSerial
	Input:	char *buf, the send data buffert
				int length, number of bytes to send.
	Output: 	-
	Calls: 	-
	Descr: 	sends the data on the serial port.

*/
int writeSerial(char *buf, int length);


#endif

