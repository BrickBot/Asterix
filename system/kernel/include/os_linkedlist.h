/*
##################################################
##  ASTERIX FRAMEWORK PROJECT                   ##
##                                              ##
## Subproject   : Asterix, realtime kernel      ##
##                                              ##
## COPYRIGHT (c) 2000                           ##
##################################################
*/
/*
**************************************************
**  File        : linkedlist.h                  **
**  Date        : 2000-03-07                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--     Misc functions to support linked list.   --
--     Each node can hold one 16 bits unsigned  --
--     integer.                                 --
--------------------------------------------------
*/

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <hal_types.h>

typedef struct _llnode{
    uint16 value;
    struct _llnode *next;
}llnode_t;        


extern void insertFirst( llnode_t *node_p , llnode_t **llistroot , llnode_t **last );
extern void insertLast( llnode_t *node_p , llnode_t **llistroot , llnode_t **last );

extern llnode_t *getFirst( llnode_t **llistroot , llnode_t **last );
extern llnode_t *getLast( llnode_t **llistroot , llnode_t **last ); 
extern llnode_t *getNode( int value , llnode_t **llistroot , llnode_t **last );


/* Mainly for debug purpose */
extern void printList( llnode_t *node_p );
        
#endif        
