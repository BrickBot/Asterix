/*----------------------------------------Serie kommunikation ------------------------------------------*/
/* Prototyp

  ! initialize the serial port and the IR transmitter/receiver.
  ! initially set to low range.

*/


#include<rcx_serial.h>
#include<rcx_undef.h>

/* values for the bit rate register BBR , error <=0.16% */
#define B2400       207
#define B4800       103
#define B9600       51
#define B19200      25
#define B38400      12

//! set IR transmitter range
/*! \param range 0: short range, 1: long range
    toggles port 4 bit 0
*/
extern inline void lnp_logical_range(int far) {
  if(far)
    *((char*)&PORT4) &=~1;
  else
    *((char*)&PORT4) |=1;
}


//! enable IR carrier frequency.
extern inline void carrier_init(void) {
  T1_CR  =0x9;
  T1_CSR =0x13;
  T1_CORA=0x1a;
}

//! shutdown IR port
void serial_shutdown(void) {
  S_CR =0;              // everything off
  lnp_logical_range(0);

}

void serialInit(void) {
  // safe initial state.
  //
  serial_shutdown();


  // serial setup to 8N1 / 2400 baud rate.
  //
  S_MR =SMR_P_NONE;
  S_BRR=B2400;
  S_SR =0;

  // carrier setup
  //
  PORT4_DDR = *(char *)0xfd83 |= 1;             // port 4 bit 0 output
  carrier_init();
  PORT5_DDR = *(char *)0xfd84 = 4;          // init p5ddr, for now

  lnp_logical_range(1);

  // enable receiver and transmiter
  //
  S_CR = SCR_TRANSMIT|SCR_RECEIVE;
}

/*
    Name:   serielError
    Descr:  a default error handeling routine, the re-inital the concerning registers
*/
void serielError(void)
{

    if( (S_SR & 0x20) == 0x20 || (S_SR & 0x10) == 0x10)
    {

        if((S_SR & 0x10) == 0x10 )
        {
            S_SR&=~0x40;
            S_SR&=~0x20;
            S_SR&=~0x10;
        }
        else{
            S_SR&=~0x20;
            S_SR&=~0x10;
        }
    }
}


/*
    Name:   readSerial
    Input:  char *buf, a buffert to to received data.
                int length, number og bytes to read.
                int block, 1; blocking read of length bytes, 0; nonblocking read.
    Output:     int, number of bytes read. In blocking mode, 1 byte is probably max.
    Calls:  serialError
    Descr:  BLOCK-read of length bytes.
                NON-BLOCK-read of data, tries to read 1 byte and return 1 if a
                byte was read.
*/
int readSerial(char *buf, int length, int block)
{
    int i;

    if(block==BLOCKED)
    {
        for(i=0;i<length;i++)
        {
            /*while bit 6 är 0*/
            while( (S_SR & 0x40) == 0)
            {
                /* felhantering */
                serielError();

            }
            /* läs byte */
            buf[i]=S_RDR;
            /*nolla bit 6 igen*/
            S_SR&=~0x40;
        }
        return 1;
    }
    else{

        serielError();

        if( (S_SR & 0x40) != 0)             /* received something*/
        {
            buf[0]=S_RDR;
            /*nolla bit 6 igen*/
            S_SR&=~0x40;
            return 1;
        }
        return 0;
    }
}

/*
    Name:   writeSerial
    Input:  char *buf, the send data buffert
                int length, number of bytes to send.
    Output:     -
    Calls:  -
    Descr:  sends a data stream on the serial port.

*/
int writeSerial(char *buf, int length)
{
    int i;

    for(i=0;i<length;i++)
    {
        /*while bit 7 är 0*/
        while( (S_SR & 0x80) == 0);

        /* skriv byte */
        S_TDR = buf[i];
          /*nolla bit 7 igen*/
        //dummy=S_SR;
        S_SR&=0x7f;

    }

    return 1;
}
