CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

AR = ar

ARFLAGS = -r

malloc: libmalloc.so libmalloc.a
	

intel-all: lib/libmalloc.so lib64/libmalloc.so lib/libmalloc.a lib64/libmalloc.a


lib/libmalloc.so: lib malloc32.o
	$(CC) $(CFLAGS) -m32 -shared -fPIC -o $@ malloc32.o

lib64/libmalloc.so: lib64 malloc64.o
	$(CC) $(CFLAGS) -fPIC -shared -o $@ malloc64.o

lib/libmalloc.a: lib malloc32.o
	$(AR) $(ARFLAGS) $@ malloc32.o

lib64/libmalloc.a: lib malloc64.o
	$(AR) $(ARFLAGS) $@ malloc64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

libmalloc.a: malloc64.o
	$(AR) $(ARFLAGS) $@ malloc64.o

libmalloc.so: malloc64.o
	$(CC) $(CFLAGS) -fPIC -shared -o $@ malloc64.o

malloc32.o: malloc.c
	$(CC) $(CFLAGS) -m32 -fPIC -c -o malloc32.o malloc.c

malloc64.o: malloc.c
	$(CC) $(CFLAGS) -fPIC -c -o malloc64.o malloc.c

test: lib/libmalloc.so
	$(CC) $(CFLAGS) -m32 -o do_test prog.c lib.c -Llib -lmalloc
	do_test

test64: lib64/libmalloc.so
	$(CC) $(CFLAGS) -o do_test64 prog.c lib.c -Llib64 -lmalloc
	do_test64

clean: 
	rm *.o
	rm -r lib lib64
