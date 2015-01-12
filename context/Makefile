CC	= gcc
CFLAGS	= -Wall -Werror -pedantic -m32
CFLAGS	+= -D_XOPEN_SOURCE=500
CFLAGS	+= -g

prodcons: prodcons.c context.o hw.o sem.o
	$(CC) $(CFLAGS) prodcons.c context.o hw.o sem.o -o prodcons

sem.o	: sem.c sem.c
	$(CC) $(CFLAGS) -c sem.c

context.o: context.c context.h
	$(CC) $(CFLAGS) -c context.c

hw.o	: hw.c hw.h
	$(CC) $(CFLAGS) -c hw.c



.PHONY	: clean realclean

clean	:
	$(RM) *~ *.o

realclean: clean
	$(RM) prodcons
