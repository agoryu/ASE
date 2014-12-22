CC	= gcc
CFLAGS	= -Wall -Werror -pedantic
CFLAGS	= -m32
CFLAGS	+= -D_XOPEN_SOURCE=500
CFLAGS	+= -g


all		: try display_stack

try		: try_mul.c try.o
		$(CC) $(CFLAGS) try_mul.c try.o -o try

try.o		: try.c try.h
		$(CC) $(CFLAGS) -c try.c

display_stack	: display_stack.c
		$(CC) $(CFLAGS) display_stack.c -o display_stack

.PHONY: clean

clean	:
	$(RM) -f *~ *.o
