/*
	Author:  Joakim Lindgren, dat95jln
	Descr :  A up-loader from the RCX, receive one message and saves it to disk,
				currently without a timeout.

	Todo: Make a state machine that reqognize a frame of data and sending ACK:s back.
			But not with full duplex.
			Now I have done it for the receive function and it seems to work.
			The last thing to do is to save the message to disk.
			Add a IR-tower response routine to.

	2000.04.25	change primitives to work under winNT and Linux.
			Still it doesn't work under winNT Cygnus.
			Anders, dte96apn.
	2000.05.18  I have debugged and tested this program under both NT and Linux
	 		and it seams quite stabile.

*/

#include <stdio.h>
#include <string.h>


#if defined(linux) || defined(LINUX)

#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <sys/time.h>

#define HANDLE	int

#define FALSE	0
#define TRUE	1

#define TTY_VARIABLE	"RCXTTY"
#define	DEFAULTTTY	"/dev/ttyS0"


extern ssize_t ReadFile(HANDLE,char *,int , unsigned long *, void * );
extern ssize_t WriteFile(HANDLE,char *,int , unsigned long *, void * );

ssize_t ReadFile(HANDLE h,char *data,int nrofchars , unsigned long *count,void *ignore)
{
	 fd_set fds;
	 int retval=0;
	 struct timeval timeout;

		  timeout.tv_sec=0;
		  timeout.tv_usec=750000;

		  FD_ZERO(&fds);
		  FD_SET(h,&fds);

		  retval = select(h+1,&fds,NULL,NULL,&timeout);

		  if(retval){
				/* Data exists, get it */
				retval = read(h,data,nrofchars);
				*count = nrofchars;
				retval = nrofchars;
		  }
		  else{
				/* No data, just return */
				*count = 0;
				retval = -1;
		  }
	return retval;
}

ssize_t WriteFile(HANDLE h,char *data,int nrofchars , unsigned long *count,void *ignore)
{

				*count = write(h,data,nrofchars);
				return *count;

/*
	 fd_set fds;
	 int retval=0;
	 struct timeval timeout;

		  timeout.tv_sec=0;
		  timeout.tv_usec=750000;

		  FD_ZERO(&fds);
		  FD_SET(h,&fds);

		  retval = select(h+1,&fds,NULL,NULL,&timeout);

		  if(retval)
				return write(h,data,nrofchars);
		  else
				return -1;
*/
}


#else  /* End of Linux specific start of NT specific */


/* Should be an ifdef for BORLAND */
#define STRICT

#include <windows.h>
#include <dos.h>

#endif



/* The binary file name */
#define PROG_NAME "communic"
int debugg=0;		/* 1 debugg on, 0 not */

/* max data size form and to the dl layer (for now anyway) */
#define DATA_SIZE 16
#define FRAME_SIZE 39 //16+1*2 +5

#define STX 0x02
#define ETX 0x03

#define DLE 0x10
#define ACK 0x06
#define NAK 0x15
#define ALIVE 0x41  	/* 'A' */

/* the states for the framing receiving machine */
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
#define STATE_4 4
#define STATE_5 5

/*

 RCX uses big endian and we want little endian

*/
void endian_convert(unsigned short *data_size)
{
	char *temp= (char*) data_size;
	char data;

	data = temp[0];
	temp[0] = temp[1];
	temp[1] = data;
}

/*
	Name: 	rec_frame
	Input:  	HANDLE h, a handle to the serial port
				char *frame_buf, a pointer to a buffer where to place the frame				int *frame_size, the size of the frame is
placed here.
	Output:  -
	Calls:   -
	Descr:  	This function reads the serial port and use a state machine to receive the frame.
*/

void rec_frame(HANDLE h, char *frame_buf, int *frame_size)			/* no timeout right now */
{

#ifdef __WIN32__
	struct  time start;
	struct  time end;
#else
	struct  timeval start;
	struct  timeval end;
#endif


	char alive=ALIVE;			/* 'A' */
	char ch;
	unsigned long count;
	int state=STATE_1;

	*frame_size=0;			/*uses both for size and index  */

#ifdef __WIN32__
	gettime(&start);
#else
	gettimeofday(&start,0);
#endif

	WriteFile(h,&alive,1,&count, NULL);

	while(1)
	{

#ifdef __WIN32__
		gettime(&end);
		if(start.ti_sec>end.ti_sec)
			start=end;
#else
		gettimeofday(&end,0);
		if(start.tv_sec>end.tv_sec)
			start=end;
#endif


#ifdef __WIN32__
		if(end.ti_sec-start.ti_sec > 3)
#else
		if(end.tv_sec-start.tv_sec > 3)
#endif
		{
			if( debugg) printf("\nKeeps the IR-tower active\n");
			WriteFile(h,&alive,1,&count, NULL);
#ifdef __WIN32__
			gettime(&start);
#else
			gettimeofday(&start,0);
#endif
		}

		count=0;
		if(ReadFile(h,&ch,1,&count,NULL) == FALSE)            /* receive one character at a time*/
			printf("Ingen data\n");

		if(count!=0)
		{
//			printf("%x ", ch);

			/* handle to big frames */
			if( (*frame_size) >= FRAME_SIZE){
				state=STATE_1;
				frame_size=0;
			}

			if(ch!=ALIVE)				/* ignore ALIVE('A'), might be my own echo */
				switch(state)
				{

					case STATE_1:
						if(ch==DLE){
							frame_buf[(*frame_size)++]=ch;
							state=STATE_2;
						}

						break;
					case STATE_2:
						if(ch==STX){
							frame_buf[(*frame_size)++]=ch;
							state=STATE_3;
						}else if(ch==DLE){
						}else{
							(*frame_size)=0;
							state=STATE_1;
						}
						break;

					case STATE_3:
						if(ch==DLE){
							frame_buf[(*frame_size)++]=ch;
							state=STATE_4;
						}else{
							frame_buf[(*frame_size)++]=ch;
						}
						break;

					case STATE_4:
						if(ch==ETX){
							frame_buf[(*frame_size)++]=ch;
							return;
						}else if(ch==STX){
							(*frame_size)=2;
							state=STATE_3;
						}else{
							frame_buf[(*frame_size)++]=ch;
							state=STATE_3;
						}

							break;

				}	/* end-switch */
		}
	}
}

/*
	Name: 	print_frame
	Input:  	char *frame_buf, a frame to print
				int frame_size, the size of the frame
	Output: 	-
	Calls:   -
	Descr:  	print the frame in hex-format
*/

void print_frame(char *frame_buf, int frame_size)
{
	int i;

	for(i=0;i<frame_size;i++)
		printf("%x ", frame_buf[i]);

	printf("\n");

}



/*
	Name:    calc_cs
	Input:  	char *data, the data to calculate the checksum for.
				int size, the size of the data
	Output:  unsigned char, the calculated checksum
	Calls:	-
	Descr:  	print the frame in dec-format

*/
unsigned char calc_cs(char *data, int size)
{
	int i,j;
	unsigned char crc_sum=0;

	for(i=0;i<size;i++)
		for(j=1;j<=128;j*=2)
			crc_sum+=!(!(data[i] & (char)j));

	return crc_sum;
//	return crc_sum%256;
}


/*
	Name: 	check_frame
	Input:	char *frame_buf, where the frame is placed
				int frame_size, the size of the input frame
				char *data_buf, where to put the data, unstuffed
				int *size, the size of the output data.
	Output:  int, 1 if ok, 2 if wrong sequential number and 0 if error in check sum.
	Calls:   calc_cs
	Descr:  	print the frame in dec-format
*/
int check_frame(char *frame_buf, int frame_size, char *data_buf, int *size)
{
	int i;
	unsigned char cs;
	char sekv;

	static char sekv_real=0;
	static int first_frame=1;

	char temp_buf[DATA_SIZE+1];	/* +1, for the checksum */

	*size=0;		/* data_buf */

	sekv=frame_buf[2];

	for(i=3;i<frame_size-2;i++)
	{
		if(frame_buf[i]==DLE)
		{
			switch(frame_buf[i+1])
			{
				case 'a':
					temp_buf[*size]=DLE;
					(*size)++;
					i++;
					break;

				case 'b':
					temp_buf[*size]=ACK;
					(*size)++;
					i++;
					break;

				case 'c':
					temp_buf[*size]=NAK;
					(*size)++;
					i++;
					break;

				case 'd':
					temp_buf[*size]=ALIVE;
					(*size)++;
					i++;
					break;

			}/* end switch */
		}
		else{
			temp_buf[*size]=frame_buf[i];
			(*size)++;
		}
	}   /*end for*/


	cs=temp_buf[0];

	/**/
	for(i=0;i<(*size)-1;i++)
		data_buf[i]=temp_buf[i+1];

	(*size)--;	/* for the checksum */

	/* check the sequential number and the checksum */
	if( debugg) printf("SEK: Got %x have: %x\n",sekv, sekv_real);

	if(first_frame){
		sekv_real=sekv;
		first_frame=0;
	}
	else
		if(sekv!=sekv_real)
			return 2;

	if( debugg) printf("CS: Got %x calc: %x\n",cs, calc_cs(data_buf, *size));
	if( debugg) printf("size= %d\n", *size);
	if(cs != calc_cs(data_buf, *size))
		return 0;

	if(sekv_real==0)
		sekv_real=1;
	else
		sekv_real=0;

	return 1;
}

/*
	Name:  	dl_receive
	Input:  	HANDLE h, a handle to the serial port
				char *data_buf, a pointer to a data buffer.
				int *size, when return, the received data-size if wrote here.
	Output:  int, 1 if a frame was received, else 0 after timeout.
	Calls:   rec_frame
				check_frame
	Descr:   tries to receive a data frame. Currently without a time out.
*/
int dl_receive(HANDLE h, char *data_buf, int *size)
{
	char frame_buf[(DATA_SIZE+1)*2+5];		/* DATA_SIZE*2 plus some overhead */
	int frame_size;
	char ack=ACK;
	char nak=NAK;
	unsigned long count;

	int status;

	while(1)
	{
		rec_frame(h,frame_buf, &frame_size);			/* no timeout right now */

		if( debugg) printf("A frame was received.\n");

//		print_frame(frame_buf, frame_size);

		if( (status=check_frame(frame_buf, frame_size, data_buf, size )) == 1 ){
			if( debugg){
				 printf("The frame was ok.\n");
						print_frame(frame_buf, frame_size);
				  }
			WriteFile(h,&ack,1,&count, NULL);
			if( debugg) printf("Wrote %ld acks.\n", count);
			return 1;
		}else if(status == 2){
			if( debugg)	printf("Wrong sequential number.\n");
			WriteFile(h,&ack,1,&count, NULL);
			if( debugg) printf("Wrote %ld acks.\n", count);
		}else if( status == 0){
			if( debugg ){
			    print_frame(frame_buf,frame_size);
			}
		}else 
		{
			if( debugg) printf("The frame was corrupted.\n");
			WriteFile(h,&nak,1,&count, NULL);
		}
	}

}

/*


	Descr: 	!!OBS!! this function dynamic allocates some memory. !!OBS!!
				It's up to the user of the function to free the memory.
*/
char* PackageReceive(HANDLE h, unsigned short *pack_size)
{
	int size;
	unsigned short data_size=0;
	unsigned short count;
	char *data;

	/* receive a data size package */

	dl_receive(h, (char*)&data_size, &size);

	endian_convert(&data_size);								/* RCX uses little endian and we want big endian */

	*pack_size=data_size;

	/* allocate a buffert for receiving data */
	data = (char*)malloc(data_size);
	/* receive the data */

	if(debugg)printf("Receiving frames: \n");

	count=0;
	while(count<data_size)
	{
		dl_receive(h, data+count, &size);
		count+=(unsigned short)size;
	}
	if(debugg)printf("Frames received\n");

	return data;
}


/*
	Name:		MemDump
	Input:   HANDLE h, a handle to the serial port
	Output: 	-
	Calls:   package_receive
	Descr: 	Dumps the data to a file as hex along with the addres. First the start
				address where the dumps start at is received in nóne package. And then the
				data in the memory is received in a secon package. Finnaly the function
				prints the data in a file in a "common" debug-style.

*/
void MemDump(HANDLE h, FILE *filpek )
{

	unsigned short pack_size;
	char *data;
	int i;
	unsigned short start_addr;
	unsigned short temp;

	data=PackageReceive(h, &pack_size);
	printf("Received a package with size %d\n", pack_size);
	if(pack_size!=2){
		perror("Received wrong package size");
		exit(1);
	}

	start_addr= *(unsigned short*)data;
	free(data);
	endian_convert(&start_addr);
	printf("Memory start address: %x\n", start_addr);

	data=PackageReceive(h, &pack_size);
	printf("Received a package with size %d\n", pack_size);


	/* Do some sort of memory dump to file */
	/* insert some padding */
	printf("Start address: 0x%04x\n", start_addr);
	temp = start_addr - (start_addr % 16);  /* an 16-even address */

	if(start_addr-temp > 0){
		fprintf(filpek,"%04x: ",temp );
	}
	for(i=0;i<(start_addr-temp);i++)
		fprintf(filpek,"   ");

	for(i=0;i<pack_size;i++)
	{
		if(i==0 && !(start_addr-temp > 0)){
			fprintf(filpek,"%04x: ", temp);			/* first row when no padding */
		}
		else if( (start_addr+i)%16==0){
			temp+=16;
			fprintf(filpek,"\n%04x: ", temp);			/* rest of lines */
		}

		/* insert the '-' charcter for each 8 byte chunk */
		if( ((i+(start_addr-temp))%16) == 8 )
		{
			if(i!=0)
				fprintf(filpek,"-", (unsigned char)data[i]);
		}
		else
			fprintf(filpek," ", (unsigned char)data[i]);

		fprintf(filpek,"%02x", (unsigned char)data[i]);
	}

	free(data);
}


#ifdef	__WIN32__

/*
	This is the main for __WIN32__ 32, not __WIN32__ Cygnus.

	Name:  	main
	Input:	int argc
				char *argv[]
				int *size, when return, the received data-size if wrote here.
	Output:  int, 1 if a frame was received, else 0 after timeout.
	Calls:   rec_frame
				check_frame
	Descr:   tries to receive a data frame. Currently without a time out.
*/
int main(int argc, char *argv[])
{
	FILE *filpek;

	if(argc>1) {
		// open port for overlapped I/O
		HANDLE h = CreateFile(argv[1],
						  GENERIC_READ|GENERIC_WRITE,
						  0,NULL,
						  OPEN_EXISTING,NULL,NULL);

		if(h == INVALID_HANDLE_VALUE) {
			printf("Failed to open port");
		} else {
			// set timeouts
			COMMTIMEOUTS cto = { 2, 1, 1, 0, 0 };
			DCB dcb;
			if(!SetCommTimeouts(h,&cto))
				printf("SetCommTimeouts failed");

			// set DCB
			memset(&dcb,0,sizeof(dcb));
			dcb.DCBlength = sizeof(dcb);
			dcb.BaudRate = 2400;
			if(argc>2 && atoi(argv[2]) )
				dcb.BaudRate = atoi(argv[2]);
			dcb.fBinary = 1;
			dcb.fDtrControl = DTR_CONTROL_ENABLE;

			dcb.fOutxCtsFlow = 0;
			dcb.fRtsControl = DTR_CONTROL_HANDSHAKE;

			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			dcb.ByteSize = 8;

			if(!SetCommState(h,&dcb))
				printf("SetCommState failed");

			if( (filpek=fopen("dump.hex","w") ) == NULL ){
				printf("Could not open the file \" dump.hex \"\n");
				exit(-1);
			}

			MemDump(h, filpek);

			CloseHandle(h);
		}
	} else {
		printf("Usage: %s [com port] [baud rate]\n", PROG_NAME);
		printf("  [com port]: e.g. com1\n");
		printf("  [baud rate]: e.g. 2400(default) or 4800\n");
	}
	return 0;


}

#else

/*
	Name:  	main
	Input:	int argc
				char *argv[]
				int *size, when return, the received data-size if wrote here.
	Output:  int, 1 if a frame was received, else 0 after timeout.
	Calls:   rec_frame
				check_frame
	Descr:   tries to receive a data frame. Currently without a time out.
*/



int main(void)
{
	 char *tty;
	 HANDLE fd;
	 struct termios ios;
	 FILE *filpek;


	printf("Default comport is %s\n",DEFAULTTTY);
		  tty = getenv(TTY_VARIABLE);
		  if (!tty)
				tty = DEFAULTTTY;

/* read from standard input if tty="-" */

		  if (*tty == '-' && !*(tty + 1))
				fd = 0;
		  else if ((fd = open(tty, O_RDWR | O_EXCL)) < 0) {
				perror("open");
				return -1;
		  }

		  if (!isatty(fd)) {
				close(fd);
				fprintf(stderr, "%s: not a tty\n", tty);
				return -1;
		  }


		  memset(&ios, 0, sizeof(ios));
		  ios.c_cflag = CREAD | CLOCAL | CS8;   // | PARENB | PARODD;


		  cfsetispeed(&ios, B2400);
		  cfsetospeed(&ios, B2400);


		  if (tcsetattr(fd, TCSANOW, &ios) == -1) {
				perror("tcsetattr");
				exit(1);
		  }


	if( (filpek=fopen("dump.hex","w") ) == NULL ){
		 printf("Could not open the file \" dump.hex \"\n");
		 exit(-1);
	}
		  MemDump(fd,filpek);
		  fflush(filpek);
		  close(fd);

		  return 0;

}

#endif
