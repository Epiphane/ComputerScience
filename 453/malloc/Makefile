CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

test: malloc.so mallocTest.o
	$(LD) $(LDFLAGS) -o mallocTest mallocTest.o -L. -lmalloc.so

mallocTest.o: mallocTest.c
	$(CC) $(CFLAGS) -c -o mallocTest.o mallocTest.c

malloc.so: malloc.o
	$(CC) -shared -fpic -o malloc.so malloc.o

malloc.o: malloc.c
	$(CC) $(CFLAGS) -c -o malloc.o malloc.c

