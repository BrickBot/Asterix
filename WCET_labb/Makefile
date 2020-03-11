##################################################
##  ASTERIX PROJECT                             ##
##################################################
#*************************************************
#**  File        : Makefile                     **
#**  Date        : 2000-06-12                   **
#**  Author(s)   : Andreas Engberg              **
#**                Anders Pettersson            **
#-------------------------------------------------
#--  Description  :                             --
#--     Makefile to build a Asterix system      --
#*************************************************

include Makefile.mk


#######################
# Default rule, invokes configuration tool, compiles and link the system
#
all:  main




#######################
# skapa coff-fil och srec-fil
#
main: main.o crt0.o
	$(LD) crt0.o main.o $(LDFLAGS) -o main.coff-h8300
	$(OBJDUMP) -D -S main.coff-h8300 >main.dis
	@echo "DONE"




########################
# Assemble the startup file.
#
crt0.o: crt0.s
	$(AS) -o crt0.o crt0.s




##########################
# Rules to compile the source code of main
#
main.o: main.c
	$(CC) -o main.o -c main.c


#########################
# Rules for cleaning up
#
clean:
	$(RM) main.o
	$(RM) crt0.o
