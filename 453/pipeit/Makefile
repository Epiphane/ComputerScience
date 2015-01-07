CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS = 

pipeit: pipeit.o
	$(LD) $(LDFLAGS) -o pipeit pipeit.o
	echo done.

pipeit.o: pipeit.c
	$(CC) $(CFLAGS) -c -o pipeit.o pipeit.c

