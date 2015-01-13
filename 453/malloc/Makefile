CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

test: malloc.a mallocTest.o
	$(LD) $(LDFLAGS) -o mallocTest mallocTest.o malloc.a

mallocTest.o: mallocTest.c
	$(CC) $(CFLAGS) -c -o mallocTest.o mallocTest.c

malloc.a: malloc.o
	ar r malloc.a malloc.o

malloc.so: malloc.o
	$(CC) -shared -fpic -o malloc.so malloc.o

malloc.o: malloc.c
	$(CC) $(CFLAGS) -c -o malloc.o malloc.c

