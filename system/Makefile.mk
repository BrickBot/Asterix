##################################################
##  ASTERIX PROJECT                             ##
##################################################
#*************************************************
#**  File        : Makefile                     **
#**  Date        : 2000-06-27                   **
#**  Author(s)   : Andreas Engberg              **
#**                Anders Pettersson            **
#-------------------------------------------------
#--  Description  : Makefile for Windows NT     --
#--  Based on makefile created by Marcus Noga   --
#*************************************************


# Path prefix
# ROOTDIR must be set to the system-path
# Uncomment the line that match your host OS


ROOTPATH		= $(ASTERIX)
SYSTEMPATH		= $(ROOTPATH)/system

ifeq ($(OSTYPE),Linux)
EXEPREFIX = h8300-hitachi-hms-
else
EXEPREFIX		= $(ROOTPATH)/h8/bin/h8300-hms-
endif

# Path to tools
NM      		= $(EXEPREFIX)nm
OBJCOPY 		= $(EXEPREFIX)objcopy
OBJDUMP 		= $(EXEPREFIX)objdump
AS      		= $(EXEPREFIX)as
AS86    		= $(EXEPREFIX)as86
CC      		= $(EXEPREFIX)gcc
CXX     		= $(EXEPREFIX)g++
LD      		= $(EXEPREFIX)ld
LD86    		= $(EXEPREFIX)ld86
AR      		= $(EXEPREFIX)ar

MAKE    		= make
RM      		= rm -f
DEL     		= del
CP      		= copy
MV      		= mv -f
REN     		= ren
TOUCH			= touch

USERPATH		= $(SYSTEMPATH)/user
KERNELPATH		= $(SYSTEMPATH)/kernel
HALPATH			= $(SYSTEMPATH)/hal
LIBPATH			= $(SYSTEMPATH)/lib

USERC			= $(USERPATH)/c
KERNELC			= $(KERNELPATH)/c
HALC			= $(HALPATH)/c

USERCPP			= $(USERPATH)/cpp
KERNELCPP		= $(KERNELPATH)/cpp
HALCPP			= $(HALPATH)/cpp

USERASM			= $(USERPATH)/asm
KERNELASM		= $(KERNELPATH)/asm
HALASM			= $(HALPATH)/asm


USERINCLUDE		= $(USERPATH)/include
KERNELINCLUDE		= $(KERNELPATH)/include
HALINCLUDE		= $(HALPATH)/include
LIBINCLUDE              = $(ROOTPATH)/system/lib/include
SYSTEMINCLUDE		= $(SYSTEMPATH)/include


BIN			= $(ROOTPATH)/bin
OBELIX			= $(BIN)/Obelix


# Path to include files
INCDIR			=	-I$(SYSTEMINCLUDE)\
				-I$(KERNELINCLUDE)\
				-I$(HALINCLUDE)\
				-I$(USERINCLUDE)\
				-I$(LIBINCLUDE)\
				-I.

# Path to libraries
LIBDIR			=	-L$(LIBPATH)\
				-L.

LIBS			=	-lrcx -lmint

# Asm-options
ASMFLAGS                =       

# C-options
COPT			=	-g -O2 -fno-builtin  -fomit-frame-pointer
CWARN			=	-Wall

CFLAGS			=	$(COPT) $(CWARN)
CXXFLAGS		=	-DCXX $(CFLAGS)


LDFLAGS			=	-fno-builtin  -T$(HALINCLUDE)/asterix_h8300.ld\
				-relax --no-whole-archive $(LIBDIR)
