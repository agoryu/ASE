CC	= gcc
CFLAGS	= -Wall -Werror -pedantic -m32
CFLAGS	+= -D_XOPEN_SOURCE=500
CFLAGS	+= -g

OBJECTS = tools.o inode.o ifile.o

TRASHFILE = *.gch *~ \#*\# nul
	
all: 	$(OBJECTS)

ifile.o: ../../lib/ifile.h ifile.c ../../lib/inode.h ../../lib/tools.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

inode.o	: inode.c ../../lib/inode.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

tools.o: tools.c ../../lib/tools.h
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
