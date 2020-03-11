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

OS_TASK_C	=	$(KERNELC)/os_tasks.c
OS_TASK_H	=	$(KERNELINCLUDE)/os_tasks.h
CONFIGFILE  	=   	$(USERPATH)/config.obx

HALOBJS		=	$(HALASM)/hal_common_Interrupt_func.o\
			$(HALASM)/hal_kernel.o\
			$(HALASM)/hal_taskswitch.o\
			$(HALASM)/hal_timer.o\
			$(HALASM)/hal_treturn.o\
			$(HALASM)/hal_yield.o\
			$(HALC)/hal_global_var.o

KERNELOBJS	=	$(KERNELC)/os_kernel.o\
			$(KERNELC)/os_axlib.o\
			$(KERNELC)/os_errorh.o\
			$(KERNELC)/os_invokeisr.o\
			$(KERNELC)/os_linkedlist.o\
			$(KERNELC)/os_semaphore.o\
			$(KERNELC)/os_signals.o\
			$(KERNELC)/os_tasks.o\
			$(KERNELC)/os_waitfree.o

USEROBJS	=	$(USERC)/irqisr.o\
			$(USERC)/userinit.o\
			$(USERC)/user.o\

LIBRCXOBJS	=	$(LIBPATH)/rcx/rcx_datalink.o\
			$(LIBPATH)/rcx/rcx_display.o\
			$(LIBPATH)/rcx/rcx_motor.o\
			$(LIBPATH)/rcx/rcx_sensor.o\
			$(LIBPATH)/rcx/rcx_serial.o\
			$(LIBPATH)/rcx/rcx_undef.o

LIBMINTOBJS	=	$(LIBPATH)/mint/divhi3.o\
			$(LIBPATH)/mint/divsi3.o\
			$(LIBPATH)/mint/modhi3.o\
			$(LIBPATH)/mint/modsi3.o\
			$(LIBPATH)/mint/mulhi3.o\
			$(LIBPATH)/mint/mulsi3.o\
			$(LIBPATH)/mint/udivhi3.o\
			$(LIBPATH)/mint/udivsi3.o\
			$(LIBPATH)/mint/umodhi3.o\
			$(LIBPATH)/mint/umodsi3.o


OBJFILES	=	$(HALOBJS) $(KERNELOBJS) $(USEROBJS)





#######################
# Default rule, invokes configuration tool, compiles and link the system
#
all:  $(OS_TASK_C) kernel.srec




#######################
# If the user configuration file is changed then run the configuration tool
#
$(OS_TASK_C): $(CONFIGFILE)
	$(OBELIX) $(CONFIGFILE)



#######################
# skapa coff-fil och srec-fil
#
kernel.srec: $(HALASM)/crt0.o $(OBJFILES)
	$(LD) $(KERNELOBJS) $(HALOBJS) $(USEROBJS) $(LIBS) $(LDFLAGS)  -o kernel.coff-h8300
	$(OBJCOPY) -I coff-h8300 -O srec kernel.coff-h8300 kernel.srec
	$(OBJDUMP) -D -S kernel.coff-h8300 >kernel.dis
	@echo "DONE"




########################
# Assemble the startup file. Loaded by the linker script asterix_h8300.ld ---   STARTUP(hal/asm/crt0.o)    ---
#
$(HALASM)/crt0.o: $(HALASM)/crt0.s
	$(AS) $(ASMFLAGS)  -o $(HALASM)/crt0.o $(HALASM)/crt0.s




######################
# Rules to compile/assemble the source code of Hardware Adaptor Layer
#
$(HALASM)/hal_common_Interrupt_func.o:	$(HALASM)/hal_common_Interrupt_func.S $(HALINCLUDE)/hal_assembler.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_common_Interrupt_func.o -c $(HALASM)/hal_common_Interrupt_func.S 


$(HALASM)/hal_kernel.o:	$(HALASM)/hal_kernel.S $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_kernel.o -c $(HALASM)/hal_kernel.S 

$(HALASM)/hal_taskswitch.o: $(HALASM)/hal_taskswitch.S $(HALINCLUDE)/hal_assembler.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_taskswitch.o -c $(HALASM)/hal_taskswitch.S 

$(HALASM)/hal_timer.o: $(HALASM)/hal_timer.S $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_timer.o -c $(HALASM)/hal_timer.S 

$(HALASM)/hal_treturn.o: $(HALASM)/hal_treturn.S $(HALINCLUDE)/hal_assembler.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_treturn.o -c $(HALASM)/hal_treturn.S 

$(HALASM)/hal_yield.o:	$(HALASM)/hal_yield.S $(HALINCLUDE)/hal_assembler.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALASM)/hal_yield.o -c $(HALASM)/hal_yield.S 

$(HALC)/hal_global_var.o:	$(HALC)/hal_global_var.c $(HALINCLUDE)/hal_kertypes.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(HALC)/hal_global_var.o -c $(HALC)/hal_global_var.c


##########################
# Rules to compile/assemble the source code of the kernel
#
$(KERNELC)/os_kernel.o: $(KERNELC)/os_kernel.c $(KERNELINCLUDE)/os_kernel.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_kernel.o -c $(KERNELC)/os_kernel.c

$(KERNELC)/os_axlib.o: $(KERNELC)/os_axlib.c $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_axlib.o -c $(KERNELC)/os_axlib.c

$(KERNELC)/os_errorh.o: $(KERNELC)/os_errorh.c $(KERNELINCLUDE)/os_kernel.h $(SYSTEMINCLUDE)/sys_spec.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_errorh.o -c $(KERNELC)/os_errorh.c

$(KERNELC)/os_invokeisr.o: $(KERNELC)/os_invokeisr.c $(KERNELINCLUDE)/os_kernel.h $(SYSTEMINCLUDE)/sys_spec.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_invokeisr.o -c $(KERNELC)/os_invokeisr.c

$(KERNELC)/os_linkedlist.o: $(KERNELC)/os_linkedlist.c $(KERNELINCLUDE)/os_linkedlist.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_linkedlist.o -c $(KERNELC)/os_linkedlist.c

$(KERNELC)/os_semaphore.o: $(KERNELC)/os_semaphore.c $(KERNELINCLUDE)/os_kernel.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_semaphore.o -c $(KERNELC)/os_semaphore.c

$(KERNELC)/os_signals.o: $(KERNELC)/os_signals.c $(KERNELINCLUDE)/os_kernel.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_signals.o -c $(KERNELC)/os_signals.c

$(KERNELC)/os_tasks.o: $(KERNELC)/os_tasks.c $(KERNELINCLUDE)/os_tasks.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_tasks.o -c $(KERNELC)/os_tasks.c

$(KERNELC)/os_waitfree.o: $(KERNELC)/os_waitfree.c $(KERNELINCLUDE)/os_kernel.h $(KERNELINCLUDE)/os_tasks.h
	$(CC) $(CFLAGS) $(INCDIR) -o $(KERNELC)/os_waitfree.o -c $(KERNELC)/os_waitfree.c



##########################
# Rules to compile/assemble the user source code
#
$(USERC)/irqisr.o: $(USERC)/irqisr.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(USERC)/irqisr.o  -c $(USERC)/irqisr.c

$(USERC)/user.o: $(USERC)/user.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(USERC)/user.o -c $(USERC)/user.c

$(USERC)/userinit.o: $(USERC)/userinit.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(USERC)/userinit.o -c $(USERC)/userinit.c




#######################
# Rules for making the libraries
#
libmint.a: $(LIBMINTOBJS)
	$(AR) -sq $(LIBPATH)/libmint.a $(LIBMINTOBJS)
	@echo done


librcx.a: $(LIBRCXOBJS)
	$(AR) -sq $(LIBPATH)/librcx.a $(LIBRCXOBJS)
	@echo done

$(LIBPATH)/rcx/rcx_datalink.o: $(LIBPATH)/rcx/rcx_datalink.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_datalink.o -c $(LIBPATH)/rcx/rcx_datalink.c


$(LIBPATH)/rcx/rcx_display.o: $(LIBPATH)/rcx/rcx_display.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_display.o -c $(LIBPATH)/rcx/rcx_display.c

$(LIBPATH)/rcx/rcx_motor.o: $(LIBPATH)/rcx/rcx_motor.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_motor.o -c $(LIBPATH)/rcx/rcx_motor.c

$(LIBPATH)/rcx/rcx_sensor.o: $(LIBPATH)/rcx/rcx_sensor.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_sensor.o -c $(LIBPATH)/rcx/rcx_sensor.c

$(LIBPATH)/rcx/rcx_serial.o: $(LIBPATH)/rcx/rcx_serial.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_serial.o -c $(LIBPATH)/rcx/rcx_serial.c

$(LIBPATH)/rcx/rcx_undef.o: $(LIBPATH)/rcx/rcx_undef.c
	$(CC) $(CFLAGS) $(INCDIR) -o $(LIBPATH)/rcx/rcx_undef.o -c $(LIBPATH)/rcx/rcx_undef.c



$(LIBPATH)/mint/divhi3.o: $(LIBPATH)/mint/divhi3.s
	$(AS)  -o $(LIBPATH)/mint/divhi3.o $(LIBPATH)/mint/divhi3.s

$(LIBPATH)/mint/divsi3.o: $(LIBPATH)/mint/divsi3.s
	$(AS)  -o $(LIBPATH)/mint/divsi3.o $(LIBPATH)/mint/divsi3.s

$(LIBPATH)/mint/modhi3.o: $(LIBPATH)/mint/modhi3.s
	$(AS)  -o $(LIBPATH)/mint/modhi3.o $(LIBPATH)/mint/modhi3.s

$(LIBPATH)/mint/modsi3.o: $(LIBPATH)/mint/modsi3.s
	$(AS)  -o $(LIBPATH)/mint/modsi3.o $(LIBPATH)/mint/modsi3.s

$(LIBPATH)/mint/mulhi3.o: $(LIBPATH)/mint/mulhi3.s
	$(AS)  -o $(LIBPATH)/mint/mulhi3.o $(LIBPATH)/mint/mulhi3.s

$(LIBPATH)/mint/mulsi3.o: $(LIBPATH)/mint/mulsi3.s
	$(AS)  -o $(LIBPATH)/mint/mulsi3.o $(LIBPATH)/mint/mulsi3.s

$(LIBPATH)/mint/udivhi3.o: $(LIBPATH)/mint/udivhi3.s
	$(AS)  -o $(LIBPATH)/mint/udivhi3.o $(LIBPATH)/mint/udivhi3.s

$(LIBPATH)/mint/udivsi3.o: $(LIBPATH)/mint/udivsi3.s
	$(AS)  -o $(LIBPATH)/mint/udivsi3.o $(LIBPATH)/mint/udivsi3.s

$(LIBPATH)/mint/umodhi3.o: $(LIBPATH)/mint/umodhi3.s
	$(AS)  -o $(LIBPATH)/mint/umodhi3.o $(LIBPATH)/mint/umodhi3.s

$(LIBPATH)/mint/umodsi3.o: $(LIBPATH)/mint/umodsi3.s
	$(AS)  -o $(LIBPATH)/mint/umodsi3.o $(LIBPATH)/mint/umodsi3.s



#########################
# Rules for cleaning up
#
realclean: clean
	$(RM) ./kernel.coff-h8300
	$(RM) ./kernel.srec
	$(RM) ./kernel.dis
	$(RM) $(OS_TASK_C)
	$(RM) $(OS_TASK_H)

clean:
	$(RM) $(HALOBJS)
	$(RM) $(KERNELOBJS)
	$(RM) $(USEROBJS)
	$(RM) $(HALASM)/crt0.o

libmintclean:
	$(RM) $(LIBPATH)/mint/*.o
	$(RM) $(LIBPATH)/libmint.a

librcxclean:
	$(RM) $(LIBPATH)/rcx/*.o
	$(RM) $(LIBPATH)/librcx.a
