CC	= gcc
CFLAGS	= -Wall -Werror -pedantic -m32
CFLAGS	+= -D_XOPEN_SOURCE=500
CFLAGS	+= -g

BINARIES =  main

TRASHFILE = *.gch *~ \#*\# nul
	
context.o: drive.c ../../lib/drive.h
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
