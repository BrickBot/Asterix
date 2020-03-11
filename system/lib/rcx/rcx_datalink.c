/*

    Communication protocoll for the Obelix Development Environment,
    Target system Lego Mindstorm RCX
    Anders Davidsson, Joakim Lindgren
    2000-02-29
*/


#include <rcx_serial.h>

#include <rcx_datalink.h>
#include <rcx_display.h>
#include <rcx_undef.h>
#include <rcx_button.h>


void packageSend(char *data, unsigned short dsize);
int DLSend(char *data, int dsize);
extern int makeFrame(char *, char *, char, int);


/*
Name:       memSend
Input:   char *data, a pointer to data to send.
            unsigned int dsize, the size of the data to send. Max 64kb
Output:  -
Calls:   packageSend
Descr:  This is a small application that send a piece of memory to the PC that when dumps is to a file.
            This function sends a package with the memory dump. To do this we first sends the address in
            one package and then the memory data.
*/
void memSend(char *data, unsigned short dsize)
{
    unsigned int ptr;

    ptr= (unsigned int)data;

    /* Send the address */
    packageSend((char*)&ptr, 2);
    /* Send the data */
    packageSend(data, dsize);
}


/*
Name:       packageSend
Input:  char *data, a pointer to the data to send
            unsigned int dsize, number of bytes to send, max 65536 bytes.
Output: -
Calls:  DLSend
Descr:  This function devides the package into the max size for frames
*/
void packageSend(char *data, unsigned short dsize)
{
    unsigned short t_size=dsize;            /* Bytes left to send */
    int count=1;

    DLSend( (char*)&dsize,2);               /* Bytes to receive */

    while(t_size>0){


        if(t_size>=16){
            DLSend(data,(int)16);
            data+=16;
            t_size-=16;
        }
        else{
            DLSend(data,(int)t_size);
            data+=t_size;
            t_size-=t_size;
        }

        setLcdNumber(LCD_UNSIGNED,count++,LCD_DECIMAL_0);
        refreshDisplay();

    }
}




void print_frame(char *frame, int fsize)
{

    short ch;
   int i;
    /* skriva ut alla tecken i framen */

    clearDisplay();
    setLcdNumber(LCD_UNSIGNED,0x6666,LCD_DECIMAL_0);
    refreshDisplay();

    ch=0;
    while(ch!=VIEW_BUTTON)
        readButtons(&ch);
    while(ch==VIEW_BUTTON)
        readButtons(&ch);

    setLcdNumber(LCD_UNSIGNED,fsize,LCD_DECIMAL_0);
    refreshDisplay();

    ch=0;
    while(ch!=VIEW_BUTTON)
        readButtons(&ch);
    while(ch==VIEW_BUTTON)
        readButtons(&ch);

    for(i=0;i<fsize;i+=2)
    {
        printHex(*(unsigned short*)&frame[i]);
        refreshDisplay();

        ch=0;
        while(ch!=VIEW_BUTTON)
            readButtons(&ch);
        while(ch==VIEW_BUTTON)
            readButtons(&ch);
    }

    clearDisplay();
    setLcdNumber(LCD_UNSIGNED,0x6666,LCD_DECIMAL_0);
    refreshDisplay();

}

/*
    Input:  int dsize, max 16 bytes
    Descr:  Sends a frame of size max 16.
*/
int DLSend(char *data2, int dsize){
  char frame_to_send[FRAME_SIZE];
  char data[16];
  char byte;
  int fsize;
  int timeouted=0;
  int done=0;
  int no_of_naks=0;
  static int seqno = 0; /* start with 0 */
  int i;
  /* copy data to a safe buffert */

  for(i=0;i<dsize;i++)
        data[i]=data2[i];

  fsize = makeFrame(data, frame_to_send, seqno, dsize);

  mDelay(5);
  writeSerial(frame_to_send, fsize);
  /*  start the timer */


  while(!done){

        while( readSerial(&byte, 1, NONBLOCKED) == 1){
            if(byte == ACK){
                done=1;
                no_of_naks=0;
            }
            else if(byte == NAK){
                writeSerial(frame_to_send, fsize);

                no_of_naks++;
                if(no_of_naks>10)
                    print_frame(data, dsize);
                timeouted=0;
            }
        }

        mDelay(1);
        timeouted++;

        if(timeouted>500){
            writeSerial(frame_to_send, fsize);
            timeouted=0;
        }
  }

  seqno = (seqno + 1) % 2;

  /*for(i = 0; i < fsize; i++)
     printf("%d: %c\n", i, frame_to_send[i]);
     printf("Leaving DLSend()... %d, %d\n", seqno, fsize);*/
  return dsize;
}

/*
  Name:     calc_cs2
  Input:    char *data, a data string
  int size, the size of data tring char *
  Output:   The checksum of the string in one char(8 bits)
  Descr:    Calculate the checksum of the inputstring of size "size".

  This function uses som boolean bitwise operation och count the
  1-set bits.
*/
unsigned char calc_cs2(char *data, int size)
{
  int i,j;
  int crc_sum=0;

  for(i=0;i<size;i++)
     for(j=1;j<=128;j*=2)
        crc_sum+=!(!(data[i] & (char)j));

  return crc_sum%256;
}



/* Builds up a frame from data
    Input: data    - the data
             frame   - the frame
      seqnr   - sequens nr. of the frame
      dsize   - size of data in bytes
    Returns the number of bytes in the frame
*/
int makeFrame(char *data, char *frame, char seqnr, int dsize){
  char temp[DATA_SIZE+1];
  char *cs_and_data;
  int i, j;
  int frame_size = 0;

  if(dsize < 1)
     return 0;

  temp[0] = calc_cs2(data, dsize);
  for(i = 0; i < dsize; i++){
     temp[i+1] = data[i];
     //  printf("%c\n", temp[i+1]);
  }

  /* just to be able to use pointer incr. */
  cs_and_data = temp;

  i = 0;
  frame[i++] = (char)0x10; /* DLE */
  frame[i++] = (char)0x2;  /* STX */
  frame[i++] = seqnr;

  /*
  if(data[0] == 0x6){ // if it is an ACK
    frame[i++] = 0x6;
    frame_size++;
  }
  else if(data[0] == 0x15){ // if it is a NAK
    frame[i++] = 0x15;
    frame_size++;
  }
  */

  for(j = dsize+1; j > 0; j--, cs_and_data++){
    //printf("cs_and_data[0] = %c, i = %d, frame_size = %d\n"
    //    , cs_and_data[0], i, frame_size);
      switch(cs_and_data[0]){
      case 0x10:  /* DLE */
    frame[i++] = (char)0x10;
    frame[i++] = (char)0x61;
    frame_size++;
    break;
      case 0x6:   /* ACK */
    frame[i++] = (char)0x10;
    frame[i++] = (char)0x62;
    frame_size++;
    break;
      case 0x15:  /* NAK */
    frame[i++] = (char)0x10;
    frame[i++] = (char)0x63;
    frame_size++;
    break;
      case 'A':   /* Alive */
    frame[i++] = (char)0x10;
    frame[i++] = (char)0x64;
    frame_size++;
    break;
      default:
    frame[i++] = cs_and_data[0];
    break;
      }
      frame_size++;
  }

  frame[i++] = 0x10; /* DLE */
  frame[i] = 0x3;    /* ETX */
  frame_size += 5;   /* 6 control-bytes was added */

  return frame_size;
}










