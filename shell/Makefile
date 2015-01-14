ROOTDIR =../libhardware

CC	= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic -m32
CFLAGS  += -std=c11
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

OBJECTS  =  commande.o

TRASHFILE = *.gch *~ \#*\# nul

all: 	$(OBJECTS) shell

# -------------------------------------------
# This line is for Makefly check
check-syntax:
	gcc $(CFLAGS) -o nul -S $(INCDIR) ${CHK_SOURCES}
# -------------------------------------------

shell: shell.c commande.o commande.h
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

commande.o: commande.c commande.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)


.PHONY:	clean cleantrash cleandoc realclean doc

cleantrash:
	$(RM) $(TRASHFILE)

clean: cleantrash
	$(RM) *.o

cleandoc:
	$(RM) -R ./html/

cleandisk:
	$(RM) vdiskA.bin vdiskB.bin

realclean: clean cleandoc cleandisk

doc:
	doxygen doxygen_log.txt