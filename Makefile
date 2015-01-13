CC	= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic -m32
CFLAGS  += -std=c11
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

TRASHFILE = *.gch *~ \#*\# nul

all: 	context file mmu shell

# -------------------------------------------
# This line is for Makefly check
check-syntax:
	gcc $(CFLAGS) -o nul -S $(INCDIR) ${CHK_SOURCES}
# -------------------------------------------

context:
	@(cd context && $(MAKE))

file:
	@(cd file && $(MAKE))

mmu:
	@(cd mmu && $(MAKE))

shell:
	@(cd shell && $(MAKE))

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