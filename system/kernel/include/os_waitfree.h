/*
##################################################
##  ASTERIX PROJECT                             ##
##                                              ##
## Subproject   : Asterix, realtime kernel      ##
##                                              ##
## COPYRIGHT (c) 2000                           ##
##################################################
*/
/*
**************************************************
**  File        : waitfree.h                    **
**  Date        : 2000-03-07                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
#ifndef __WAIT_FREE__
#define __WAIT_FREE__

#include <hal_types.h>
#include <hal_kertypes.h>
#include <os_linkedlist.h>


#define WF_WRITER   0
#define WF_READER   1

#define WF_NOTWRITTEN  FALSE
#define WF_WRITTEN     TRUE

/* This should be created from Obelix and be placed in os_tasks.h */
typedef int16   buffertype_t;
typedef uint16  bufferid_t;

typedef struct
{
    buffertype_t          *buf;
/*    void        *buf;     */      /* buf[] */
    uint16          *bufsemaphore;  /* bufsemaphore[] */
    llnode_t        *wfoldp;        /* waitfree oldest pointer */
    llnode_t        *last;          /* latest value or writer */

}buffer_t;

typedef struct
{
    uint16  buf_id;                 /* which buffer ? */
    uint8   access;                 /* reader or writer */
    uint8   written;                /* true if value have been written into the buffer */
    uint16  bufindex;               /* the current index in the specific buffer */
}bufctrl_t;

extern buffer_t *buffer;

void updateWaitfree(tcbid_t tcbidx);
void t_returnWaitfree(tcbid_t tcbidx);
buffertype_t *getReadPointerWF(bufferid_t id);
buffertype_t *getWritePointerWF(bufferid_t id);
void writeChannel( buffertype_t *ut, uint16 value, bufferid_t channel_id );

#endif
