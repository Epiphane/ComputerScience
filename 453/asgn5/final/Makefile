CC = gcc

CFLAGS = -Wall -g -I .

LD = gcc

LDFLAGS = -Wall -g -L.

all: minget minls

clean:	
	rm -f minget minls *.o

minget: minget.o file.o 
	$(LD) $(LDFLAGS) -o minget minget.o file.o

minls: minls.o file.o 
	$(LD) $(LDFLAGS) -o minls minls.o file.o

minget.o: minget.c
	$(CC) $(CFLAGS) -c minget.c

minls.o: minls.c
	$(CC) $(CFLAGS) -c minls.c

file.o: file.c
	$(CC) $(CFLAGS) -c file.c
