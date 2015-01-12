ROOTDIR =../libhardware
#ROOTDIR =/home/enseign/ASE

CC	= gcc
CFLAGS	= -Wall -Werror -ansi -pedantic -m32 
CFLAGS  += -std=c11
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

OBJ_LIB	 =  drive.o mbr.o vol.o inode.o ifile.o
OBJ_LIB  += dir.o file.o
OBJECTS  =  $(OBJ_LIB)

TRASHFILE = *.gch *~ \#*\# nul

all: 	$(OBJECTS)


# -------------------------------------------
# This line is for Makefly check
check-syntax:
	gcc $(CFLAGS) -o nul -S $(INCDIR) ${CHK_SOURCES}
# -------------------------------------------


# Cinquième couche: système de noms de fichier
file.o: file.h file.c ifile.h dir.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)
dir.o: dir.c dir.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

# Quatrième couche: manipulation de fichiers
ifile.o: ifile.h ifile.c inode.h tools.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

tools.o: tools.c tools.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

# Troisième couche (2e partie): structure d'un fichier
inode.o: inode.h inode.c
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

# Troisième couche (1ere partie): structure d'un volume
vol.o: vol.h vol.c
	$(CC) $(CFLAGS) -c $^ $(INCDIR)

# Deuxieme couche: gestion de volumes
mbr.o:  mbr.h mbr.c drive.h
	$(CC) $(CFLAGS) -c $^ $(INCDIR)


# Premiere couche: acces materiel
drive.o: drive.h drive.c
	$(CC) $(CFLAGS) -c $^ $(INCDIR)





.PHONY:	clean cleantrash cleandoc cleandisk realclean doc

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
