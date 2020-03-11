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
SYSTEMPATH		= $(ROOTPATH)/WCET_labb

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

BIN			= $(ROOTPATH)/bin


# C-options
COPT			=	-g -O2 -fno-builtin  -fomit-frame-pointer
CWARN			=	-Wall

CFLAGS			=	$(COPT) $(CWARN)
CXXFLAGS		=	-DCXX $(CFLAGS)


#LDFLAGS		=	-fno-builtin  -T$(HALINCLUDE)/asterix_h8300.ld -relax --no-whole-archive $(LIBDIR)

LDFLAGS			=	-fno-builtin -relax  -e _start