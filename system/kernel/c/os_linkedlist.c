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
**  File        : linkedlist.c                  **
**  Date        : 2000-05-10                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--                                              --
--------------------------------------------------
*/

#include <stdlib.h>
#include <os_linkedlist.h>


void insertFirst( llnode_t *node_p , llnode_t **llistroot , llnode_t **last )
{

        if( *llistroot == NULL ){
            node_p->next = NULL;
            *last = node_p;
        }else{
            node_p->next = *llistroot;
        }

        *llistroot = node_p;
        return;
}


void insertLast( llnode_t *node_p , llnode_t **llistroot , llnode_t **last  )
{

        node_p->next = NULL;


        if( *llistroot == NULL ){
            *llistroot = node_p;
            *last = node_p;
        }else{
            (*last)->next = node_p;
            *last = node_p;
        }
        return ;
}


llnode_t *getFirst( llnode_t **llistroot , llnode_t **last  )
{
    llnode_t *node_p=NULL;

        if( *llistroot == NULL )
            return NULL;

        node_p = *llistroot;
        *llistroot = node_p->next;
        node_p->next = NULL;

        /* There is only one entry in the list last is equal llistroot */
        if( (*llistroot)->next == NULL )
            *last = *llistroot;

        return node_p;
}

llnode_t *getLast( llnode_t **llistroot , llnode_t **last  )
{
        return( getNode( (*last)->value , llistroot , last ));
}


llnode_t *getNode( int value , llnode_t **llistroot , llnode_t **last )
{
    llnode_t *node_p = NULL;
    llnode_t *nextlast_p = NULL;
    llnode_t *traverse_p = *llistroot;

        /* If the list is empty no node can be returned */
        if( *llistroot == NULL ){
            return NULL;
        }


        /* If the value match the value in the first node */
        /* else go trough the  list */
        if( traverse_p->value == value ){
            node_p = getFirst( llistroot , last );
            return node_p;
        } else {
            while( traverse_p->next != NULL ){
                if( traverse_p->next->value == value ){
                    node_p = traverse_p;
                }
                nextlast_p = traverse_p;
                traverse_p=traverse_p->next;
            }

            /* Remove the node from the list, and set node_p to appropiate return value */
            traverse_p = node_p->next;
            node_p->next = node_p->next->next;
            node_p = traverse_p;

        } 

        /* The last node was removed update pointer to the new last node */
        if( node_p->next == NULL )
            *last = nextlast_p;

        node_p->next = NULL;

        return node_p;
}
