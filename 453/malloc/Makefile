CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

intel-all: lib/libmalloc.so lib64/libmalloc.so

lib/libmalloc.so: lib malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ malloc32.o

lib64/libmalloc.so: lib64 malloc64.o
	$(CC) $(CFLAGS) -shared -o $@ malloc64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -m64 -c -o malloc64.o malloc.c

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

