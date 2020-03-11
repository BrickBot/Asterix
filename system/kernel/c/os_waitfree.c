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
**  File        : waitfree.c                    **
**  Date        : 2000-02-27                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--    The implementation of a simplified        --
--    algorithm of wait- and lock-free channels.--
--    This implementation only supports one     --
--    writer per wait- and lock-free channel.   --
--                                              --
--------------------------------------------------
*/
#include <os_kernel.h>

#ifdef  __WAITFREE__

/*
**************************************************
**  Name        :                               **
**     llnode_t *getOldestUnused(               **
**       buffer_t channel, llnode_t list,       **
**       llnode_t lastnode);                    **
**                                              **
**  Usage       :                               **
**     free_buffer = getOldestUnused(Channel,   **
**                        List, LastNode);      **
**                                              **
**  Arguments   :                               **
**              buffer_t channel, the channel   **
**                in which a free buffer is     **
**                wanted.                       **
**              llnode_t list, root node in the **
**                list where pointers to all the**
**                available buffers are located.**
**              llnode_t last, the last node in **
**                the list.                     **
**                                              **
**  Returns     : A pointer to a free buffer    **
**                                              **
**                                              **
**  Purpose     : Used by the writer to aquire  **
**                a free buffer to write the    **
**                new value(s) in.              **
**                                              **
**  Use         :                               **
**                getFirst();                   **
**                getNode();                    **
**                                              **
**************************************************
*/
llnode_t *getOldestUnused( buffer_t *buf , llnode_t **list , llnode_t **last )
{
    llnode_t    *node_p     = NULL;
    llnode_t    *tmpnode_p  = (*list)->next;
    uint16      bufferidx   = 0;
    uint16      found       = FALSE;


        bufferidx = (*list)->value;
        if( buf->bufsemaphore[ bufferidx ] == 0 ){
            node_p = getFirst( list , last );
        }else{
            while( tmpnode_p != NULL ){
                bufferidx = tmpnode_p->value;
                if( (buf->bufsemaphore[ bufferidx ] == 0) && (found == FALSE) ){

                    node_p = getNode( bufferidx , list , last );
                    found = TRUE;

                }
            }
        }

       return node_p;
}
/*
**************************************************
**  Name        :                               **
**     llnode_t *getLatest( buffer_t channel,   **
**       llnode_t list, llnode_t lastnode);     **
**                                              **
**  Usage       :                               **
**     free_buffer = getLatest(Channel, List    **
**                             LastNode);       **
**                                              **
**  Arguments   :                               **
**              buffer_t channel, the channel   **
**                in which a free buffer is     **
**                wanted.                       **
**              llnode_t list, root node in the **
**                list where pointers to all the**
**                available buffers are located.**
**              llnode_t last, the last node in **
**                the list.                     **
**                                              **
**  Returns     : A pointer to the most recently**
**                updated buffer.               **
**                                              **
**  Purpose     : Used by a reader to aquire    **
**                the buffer with most recently **
**                updated value to read from.   **
**                                              **
**  Use         :                               **
**                getLast();                    **
**                insertLast();                 **
**                                              **
**************************************************
*/

llnode_t *getLatest( buffer_t *buf , llnode_t **list , llnode_t **last )
{
    llnode_t    *node_p     = NULL;
    llnode_t    *tmpnode_p  = NULL;

    uint16 bufferidx = 0;

        bufferidx = (*last)->value;
        /*
          If no writer owns the last node 
               return a pointer to the last node.
          else
               return the second last node.
        */
        if( buf->bufsemaphore[ bufferidx ] == 0 ){
	  return *last;
/*            node_p = getLast( list , last );
*/
        }else{
	  tmpnode_p=*list;
	  while(tmpnode_p->next->value!=bufferidx)
	    tmpnode_p=tmpnode_p->next;

	  return tmpnode_p;
/*
            tmpnode_p = getLast( list , last );
            node_p    = getLast( list , last );
            insertLast( tmpnode_p , list , last );
*/
        }

        return node_p;
}

/*
**************************************************
**  Name        : void updateWaitfree( tcbid_t )**
**  Usage       : updateWaitfree( tcbidx );     **
**  Arguments   : An index to a task in the     **
**                TCBList.                      **
**  Returns     : none                          **
**                                              **
**  Purpose     : Assign new buffers to tasks   **
**                when a new period occurs or   **
**                when awakened by a signal.    **
**                                              **
**  Use         : getOldestUnused();            **
**                insertLast();                 **
**                getLatest();                  **
**                                              **
**************************************************
*/
void updateWaitfree(tcbid_t tcbidx)
{
    uint16      i=0;
    bufctrl_t    *bufctrl=TCBList[tcbidx].bufpointer;

    uint16      total=bufctrl->buf_id;
    buffer_t    *loc_buf=NULL;
    llnode_t    *llpost=NULL;


        bufctrl++;


        for(i=0;i<total;i++,bufctrl++)
        {
            loc_buf=&buffer[bufctrl->buf_id];

            if( (bufctrl->access == WF_WRITER) && ( bufctrl->written == TRUE ) )
            {
                llpost=getOldestUnused(loc_buf,&(loc_buf->wfoldp),&(loc_buf->last));
                insertLast(llpost,&(loc_buf->wfoldp),&(loc_buf->last));
	        bufctrl->written = WF_NOTWRITTEN;
            }
            else
            {
                llpost=getLatest(loc_buf,&(loc_buf->wfoldp),&(loc_buf->last));
            }
            loc_buf->bufsemaphore[llpost->value]++;
            bufctrl->bufindex=llpost->value;
        }
}

/*
**************************************************
**  Name        : void t_returnWaitFree(tcbid_t)**
**  Usage       : t_returnWaitFree( tcbidx );   **
**  Arguments   : An index to a task in the     **
**                TCBList.                      **
**  Returns     : none                          **
**                                              **
**  Purpose     : Assign new buffers, when task **
**                have terminated.              **
**                                              **
**  Use         : getNode();                    **
**                insertLast();                 **
**                                              **
**************************************************
*/
void t_returnWaitfree(tcbid_t tcbidx)
{
    uint16      i;
    bufctrl_t    *bufctrl=TCBList[tcbidx].bufpointer;

    uint16      total=bufctrl->buf_id;
    buffer_t    *loc_buf=NULL;
    llnode_t    *llpost=NULL;


    bufctrl++;


    for( i=0; i<total; i++,bufctrl++ )
    {
        loc_buf=&buffer[bufctrl->buf_id];

        loc_buf->bufsemaphore[bufctrl->bufindex]--;

        if( (bufctrl->access == WF_WRITER) && ( bufctrl->written == TRUE ) )
        {
            llpost=getNode(bufctrl->bufindex,&(loc_buf->wfoldp),&(loc_buf->last));
            insertLast(llpost,&(loc_buf->wfoldp),&(loc_buf->last));
            bufctrl->written = WF_NOTWRITTEN;
        }
    }
}

/*
**************************************************
**  Name        :  buffertype_t *getReadPointerWF(bufferid_t id)**
**  Usage       :  getReadPointerWF( channel_n );   **
**  Arguments   :  The identity of the channel  **
**  Returns     :  A pointer to a buffer.       **
**                                              **
**  Purpose     :  Used by a reader to get a    **
**                 pointer to the buffer where  **
**                 the value to ber read is.    **
**                                              **
**  Use         :  none                         **
**                                              **
**************************************************
*/
buffertype_t *getReadPointerWF(bufferid_t id)
{
    tcbid_t     tcbidx=ExecTask;
    bufctrl_t   *bufctrl=TCBList[tcbidx].bufpointer;

    uint16      i;
    uint16      total=bufctrl->buf_id;
    uint16      bufid;
    uint16      index;

    hal_disable_interrupt();

    bufctrl++; /* the first index bufctrl[0] is just a dummy for storing array information */

    for(i=0;i<total;i++,bufctrl++)
    {
        if(id==bufctrl->buf_id && bufctrl->access==WF_READER)
        {
            bufid=bufctrl->buf_id;
            index=bufctrl->bufindex;
            hal_enable_interrupt();
            return &(buffer[bufid].buf[index]);
        }
    }
    hal_enable_interrupt();
    return NULL;
}

/*
**************************************************
**  Name        : buffertype_t *getWritePointerWF(bufferid_t id) **
**  Usage       :  getWritePointerWF( channel_n );  **
**  Arguments   :  The identity of the channel  **
**  Returns     :  A pointer to trhe buffer     **
**                                              **
**  Purpose     :  Used by the writer to get a  **
**                 pointer to a buffer where to **
**                 write the new value.         **
**                                              **
**  Use         :  none                         **
**                                              **
**************************************************
*/
buffertype_t *getWritePointerWF(bufferid_t id)
{
    tcbid_t     tcbidx=ExecTask;
    bufctrl_t   *bufctrl=TCBList[tcbidx].bufpointer;

    uint16      i;
    uint16      total=bufctrl->buf_id;
    uint16      bufid;
    uint16      index;

    hal_disable_interrupt();

    bufctrl++; /* the first index bufctrl[0] is just a dummy for storing array information */

    for(i=0;i<total;i++,bufctrl++)
    {
        if(id==bufctrl->buf_id && bufctrl->access==WF_WRITER )
        {
            bufid=bufctrl->buf_id;
            index=bufctrl->bufindex;

            hal_enable_interrupt();
            return &(buffer[bufid].buf[index]);

        }
    }
    hal_enable_interrupt();
    return NULL;
}

void writeChannel( buffertype_t *ut, uint16 value , bufferid_t channel_id )
{
    taskid_t task_id;
    bufctrl_t *bufctrl;
    bufferid_t nof_entries;
    uint16 i;

        task_id = self();
	bufctrl =TCBList[task_id].bufpointer;
	nof_entries = bufctrl->buf_id; /* The dummy entry contains the number of total entries iin the array */
	bufctrl++;  /* the first index bufctrl[0] is just a dummy for storing array information */


        for( i=0; i<nof_entries ;i++,bufctrl++)
        {
            if(channel_id == bufctrl->buf_id && bufctrl->access==WF_WRITER )
            {
                bufctrl->written = WF_WRITTEN;
                *ut = value;
            }
        }
}

#endif  /* __WAITFREE__ */
