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
**  File        : types.h                       **
**  Date        : 2000-03-07                    **
**  Author(s)   : Andreas Engberg               **
**                Anders Pettersson             **
**************************************************
*/
/*
--------------------------------------------------
-- Description  :                               --
--------------------------------------------------
*/
#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned long int       uint64;
typedef long int                int64;
typedef unsigned int            uint32;
typedef int                     int32;
typedef unsigned short int      uint16;
typedef short int               int16;
typedef unsigned char           uint8;
typedef char                    int8;

typedef void (*void_func_p)(void *);

#endif	/* __TYPES_H__ */
