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
CFLAGS	= -Wall -Werror #-Wextra
CFLAGS  += -ansi -pedantic -m32
#CFLAGS	+= -O3 -pipe -std=gnu11
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

SUBDIRS  = mmu filesys drive context hw shell

BINARIES = prodcons dmps strs frmt mkvol dfs mknfs shell testCore
BINPATHS = ${addprefix $(BINDIR)/, $(BINARIES)}

OBJECTS  = ${addsuffix .o, $(BINARIES)}
OBJECTS += ${addprefix mmu/, ${addsuffix .o, mi_kernel mi_user}}
OBJECTS += ${addprefix filesys/, \
	   ${addsuffix .o, mbr super inode ifile dir file}}
OBJECTS += ${addprefix drive/, ${addsuffix .o, drive}}
OBJECTS += ${addprefix context/, ${addsuffix .o, context sem}}
OBJECTS += ${addprefix hw/, ${addsuffix .o, hw}}
OBJECTS += ${addprefix shell/, ${addsuffix .o, commande}}
OBJPATHS = ${addprefix $(OBJDIR)/, $(OBJECTS)}
OBJDIRS  = ${addprefix $(OBJDIR)/, $(SUBDIRS)}

RESSOURCES = hardware.ini core.ini

LIBRARIES = lib/libhardware.a

LIBFLAGS = -L$(LIBDIR) -lhardware -lpthread


###------------------------------
### Main targets 
###------------------------------------------------------------
all: $(BINPATHS) $(OBJPATHS) $(RESSOURCES) $(LIBRARIES) bin/mmutest


###------------------------------
### Make binaries
###------------------------------------------------------------
bin/testCore:\
	obj/context/context.o obj/testCore.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/mmutest:\
	obj/mmu/mi_user.o obj/mmu/mi_kernel.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/shell:\
	obj/shell.o obj/shell/commande.o obj/filesys/tools.o \
	obj/filesys/file.o obj/filesys/dir.o obj/filesys/ifile.o \
	obj/filesys/inode.o obj/filesys/super.o obj/filesys/mbr.o \
	obj/drive/drive.o obj/hw/hw.o \
	obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/mknfs:\
	obj/mknfs.o obj/filesys/tools.o obj/filesys/dir.o \
	obj/filesys/ifile.o obj/filesys/inode.o \
	obj/filesys/super.o obj/filesys/mbr.o \
	obj/drive/drive.o obj/hw/hw.o \
	obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/dfs:\
	obj/dfs.o obj/filesys/inode.o obj/filesys/super.o \
	obj/filesys/mbr.o obj/drive/drive.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/mkvol:\
	obj/mkvol.o obj/filesys/inode.o obj/filesys/super.o \
	obj/filesys/mbr.o obj/drive/drive.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/frmt:\
	obj/frmt.o obj/drive/drive.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/strs:\
	obj/strs.o obj/drive/drive.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/dmps:\
	obj/dmps.o obj/drive/drive.o \
	obj/hw/hw.o obj/context/context.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin/prodcons:\
	obj/prodcons.o obj/context/sem.o \
	obj/context/context.o obj/hw/hw.o | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

bin:
	mkdir -p $(BINDIR)


###------------------------------
### Compile source
###------------------------------------------------------------
obj/testCore.o:		testCore.c
obj/shell.o:		shell.c shell/commande.h
obj/writefile.o:	writefile.c filesys/inode.h filesys/ifile.h filesys/tools.h
obj/readfile.o:		readfile.c filesys/inode.h filesys/ifile.h filesys/tools.h
obj/mkfile.o:		mkfile.c filesys/inode.h filesys/ifile.h filesys/tools.h
obj/mknfs.o:		mknfs.c filesys/super.h
obj/dfs.o:		dfs.c filesys/mbr.h
obj/mkvol.o:		mkvol.c filesys/mbr.h
obj/frmt.o:		frmt.c drive/drive.h
obj/strs.o:		strs.c drive/drive.h
obj/dmps.o:		dmps.c drive/drive.h
obj/prodcons.o: 	prodcons.c context/sem.h

obj/mmu/mi_user.o:	mmu/mi_user.c mmu/mi_syscall.h
obj/mmu/mi_kernel.o:	mmu/mi_kernel.c mmu/mi_syscall.h

obj/shell/commande.o: 	shell/commande.c shell/commande.h

obj/filesys/file.o:	filesys/file.c filesys/file.h
obj/filesys/dir.o:	filesys/dir.c filesys/dir.h filesys/ifile.h filesys/tools.h filesys/super.h
obj/filesys/ifile.o:	filesys/ifile.c filesys/ifile.h
obj/filesys/inode.o:	filesys/inode.c filesys/inode.h
obj/filesys/super.o:	filesys/super.c filesys/super.h
obj/filesys/mbr.o:	filesys/mbr.c filesys/mbr.h

obj/drive/drive.o:	drive/drive.c drive/drive.h

obj/context/sem.o: 	context/sem.c context/sem.h
obj/context/context.o: 	context/context.c context/context.h

obj/hw/hw.o: 		hw/hw.c hw/hw.h hw/hardware.h context/context.h


obj/%.o: %.c | $(OBJDIRS)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ -c $<


###------------------------------
### Make object directories
###------------------------------------------------------------
obj/mmu:
	mkdir -p $(OBJDIR)
	mkdir -p $@

obj/filesys:
	mkdir -p $(OBJDIR)
	mkdir -p $@

obj/drive:
	mkdir -p $(OBJDIR)
	mkdir -p $@

obj/context:
	mkdir -p $(OBJDIR)
	mkdir -p $@

obj/hw:
	mkdir -p $(OBJDIR)
	mkdir -p $@

obj/shell:
	mkdir -p $(OBJDIR)
	mkdir -p $@


###------------------------------
### Makefly checker
###------------------------------------------------------------
check-syntax:
	$(CC) $(CFLAGS) -I$(INCDIR) -o nul -S $(CHK_SOURCES) 


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean cleantrash realclean doc cleandoc

cleantrash:
	${foreach trash,$(TRASHFILE),\
		find . -name "$(trash)" -type f -delete;\
	}

clean: cleantrash
	$(RM) -R $(OBJDIR)

cleanbin:
	$(RM) -R $(BINDIR)

cleandisk:
	$(RM) vdiskA.bin vdiskB.bin

cleandoc:
	$(RM) -R $(DOCDIR)

realclean: clean cleanbin cleandisk

doc: doxygen.ini
	doxygen $<
