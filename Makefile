# -------------------------------------------
# Description
#
# Author: 
# 	gaetan.deflandre@gmail.com
#	elliot.vanegue@gmail.com
# Date	: 16-01-2015
# -------------------------------------------


###------------------------------
### Compile options
###------------------------------------------------------------
CC	= gcc
CFLAGS	= -Wall -Werror -Wextra
CFLAGS  += -ansi -pedantic -m32
CFLAGS  += -std=gnu11
CFLAGS	+= -g

LDFLAGS = -m32


TRASHFILE = *.gch *~ \#*\# nul
RM	  = rm -f


###------------------------------
### Paths
###------------------------------------------------------------
.SUFFIXES: .c .o

SRCDIR	= src
INCDIR	= include
OBJDIR	= obj
LIBDIR	= lib 
BINDIR	= bin
ETCDIR	= etc
DOCDIR	= doc  

vpath %.c $(SRCDIR)
vpath %.h $(INCDIR)
vpath %.ini $(ETCDIR)

BINARIES = prodcons dmps strs frmt dvol mkvol
BINPATHS = ${addprefix $(BINDIR)/, $(BINARIES)}

OBJECTS  = ${addsuffix .o, $(BINARIES)}
OBJECTS += ${addprefix mmu/, ${addsuffix .o, mi_kernel mi_user}}
OBJECTS += ${addprefix filesys/, ${addsuffix .o, mbr vol}}
OBJECTS += ${addprefix drive/, ${addsuffix .o, drive}}
OBJECTS += ${addprefix context/, ${addsuffix .o, context sem}}
OBJECTS += ${addprefix hw/, ${addsuffix .o, hw}}
OBJPATHS = ${addprefix $(OBJDIR)/, $(OBJECTS)}

RESSOURCES = hardware.ini

LIBRARIES = lib/libhardware.a

LIBFLAGS = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets 
###------------------------------------------------------------
all: $(BINPATHS) $(OBJPATHS) $(RESSOURCES) $(LIBRARIES) bin/mmutest

###------------------------------
### Make binaries
###------------------------------------------------------------
bin/mmutest:\
	obj/mmu/mi_user.o obj/mmu/mi_kernel.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/mkvol:\
	obj/mkvol.o obj/filesys/vol.o \
	obj/filesys/mbr.o obj/drive/drive.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/dvol:\
	obj/dvol.o obj/filesys/vol.o \
	obj/filesys/mbr.o obj/drive/drive.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/frmt:\
	obj/frmt.o obj/drive/drive.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/strs:\
	obj/strs.o obj/drive/drive.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/dmps:\
	obj/dmps.o obj/drive/drive.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/prodcons:\
	obj/prodcons.o obj/context/sem.o \
	obj/context/context.o obj/hw/hw.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

bin:
	mkdir -p $(BINDIR)

###------------------------------
### Compile source
###------------------------------------------------------------
obj/mkvol.o:		mkvol.c filesys/vol.h filesys/mbr.h drive/drive.h
obj/dvol.o:		dvol.c filesys/vol.h filesys/mbr.h drive/drive.h
obj/frmt.o:		frmt.c drive/drive.h hw/hardware.h
obj/strs.o:		strs.c drive/drive.h hw/hardware.h
obj/dmps.o:		dmps.c drive/drive.h hw/hardware.h
obj/prodcons.o: 	prodcons.c context/sem.h context/context.h hw/hw.h

obj/mmu/mi_user.o:	mmu/mi_user.c mmu/mi_syscall.h
obj/mmu/mi_kernel.o:	mmu/mi_kernel.c mmu/mi_syscall.h

obj/filesys/vol.o:	filesys/vol.c filesys/vol.h filesys/mbr.h drive/drive.h
obj/filesys/mbr.o:	filesys/mbr.c filesys/mbr.h drive/drive.h

obj/drive/drive.o:	drive/drive.c drive/drive.h hw/hardware.h

obj/context/sem.o: 	context/sem.c context/sem.h hw/hw.h
obj/context/context.o: 	context/context.c context/context.h hw/hw.h

obj/hw/hw.o: 		hw/hw.c hw/hw.h



$(OBJDIR) obj/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ -c $<


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean cleantrash realclean doc cleandoc

cleantrash:
	${foreach trash,$(TRASHFILE),\
		find . -name "$(trash)" -type f -delete;\
	}

clean: cleantrash
	$(RM) $(OBJPATHS)

cleanbin:
	$(RM) -R $(BINDIR)

cleandisk:
	$(RM) vdiskA.bin vdiskB.bin

cleandoc:
	$(RM) -R $(DOCDIR)

realclean: clean cleanbin cleandisk cleandoc

doc: doxygen.ini
	doxygen $<
