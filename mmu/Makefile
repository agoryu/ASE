#ROOTDIR =../libhardware
ROOTDIR =/home/enseign/ASE

CC	= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic
CFLAGS  += -std=c11
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

OBJECTS  =  mmu_manager.o swap.o

TRASHFILE = *.gch *~ \#*\# nul

all: 	$(OBJECTS)

# -------------------------------------------
# This line is for Makefly check
check-syntax:
	gcc $(CFLAGS) -o nul -S $(INCDIR) ${CHK_SOURCES}
# -------------------------------------------

mmu_manager.o: mmu_manager.c mmu_manager.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

swap.o: swap.c swap.h
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
