CC = g++

CFLAGS = -Wall -g

LD = g++

LDFLAGS = 

cagethe: Image.o cage.o tiny_obj_loader.o main.o
	$(LD) $(LDFLAGS) -o cagethe Image.o cage.o tiny_obj_loader.o main.o

tiny_obj_loader.o: tiny_obj_loader.cc
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm *.o
