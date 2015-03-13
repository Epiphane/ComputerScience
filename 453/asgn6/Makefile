# Hello.c
# Greet the world
# CPE 453 - Asgn 6
# Thomas Steinke

CC = gcc

CFLAGS = -Wall -ansi -pedantic

LD = gcc

LDFLAGS =

all: hello

clean:
	rm hello.o

hello: hello.o
	$(LD) $(LDFLAGS) -o hello hello.o

hello.o: hello.c
	$(CC) $(CFLAGS) -c -o hello.o hello.c

test: hello
	@echo Testing hello
	@echo ------------------
	@./hello
	@echo ------------------
	@echo done.
