CC = gcc

CFLAGS = -Wall -g -I .

LD = gcc

LDFLAGS = -Wall -g -L.

all: dine

clean:	
	rm -f dine *.o

dine: dine.o 
	$(LD) $(LDFLAGS) -o dine dine.o -lpthread -lrt

dine.o: dine.c
	$(CC) $(CFLAGS) -c dine.c
