#
# Makefile for the Obelix Configuration Compiler
# Anders Davidsson 00-02-25
# Revision 00-05-18, new target dos2unix
#

CC=gcc -I.

SOURCES=lexyy.c y.tab.c symbol.c syntree.c semact.c emit.c eutils.c
OBJECTS=$(SOURCES:.c=.o)

CWARN=-g -Wall
COPT=

CFLAGS=$(COPT) $(CWARN)

Obelix: $(OBJECTS)
	@bin.scr
	$(CC) $(CFLAGS) -o ../../bin/Obelix $(OBJECTS)

Lexyy.o: lexyy.c 
	$(CC) $(CFLAGS) -c lexyy.c

y.tab.o: y.tab.c 
	$(CC) $(CFLAGS) -c y.tab.c

symbol.o: symbol.c symbol.h Obelix.h
	$(CC) $(CFLAGS) -c symbol.c

syntree.o: syntree.c syntree.h
	$(CC) $(CFLAGS) -c syntree.c

semact.o: semact.c semact.h syntree.h Obelix.h
	$(CC) $(CFLAGS) -c semact.c

emit.o: emit.c Obelix.h
	$(CC) $(CFLAGS) -c emit.c

eutils.o: eutils.c eutils.h
	$(CC) $(CFLAGS) -c eutils.c

lexyy.c: Obelix.lex y.tab.c y.tab.h Obelix.h syntree.h
	flex  -olexyy.c Obelix.lex

y.tab.c: Obelix.y Obelix.h syntree.h symbol.h semact.h
	bison -d -o y.tab.c Obelix.y

realclean: clean	
	rm *~

clean:
	rm y.tab.c y.tab.h lexyy.c $(OBJECTS)

dos2unix:	
	dos2unix *.* Makefile



