CC	= gcc
CFLAGS	= -Wall -Werror -pedantic -m32
CFLAGS	+= -D_XOPEN_SOURCE=500
CFLAGS	+= -g

BINARIES =  main

TRASHFILE = *.gch *~ \#*\# nul

#main: main.o context.o
#	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

#main.o: main.c $(HEADER_PATH)context.h
#	$(CC) $(CFLAGS) -c $^ $(INCDIR)
	
context.o: context.c ../../lib/context.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

hw.o	: hw.c ../../lib/hw.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)


.PHONY:	clean cleantrash cleandoc cleanbin realclean doc

cleantrash:
	$(RM) $(TRASHFILE)

clean: cleantrash
	$(RM) *.o

cleandoc:
	$(RM) -R ./html/

cleanbin:
	$(RM) $(BINARIES)

realclean: clean cleandoc cleanbin cleandisk

doc:
	doxygen doxygen_log.txt
