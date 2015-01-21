#TESTING
PLNICO	= libPLN.a

CC 	= gcc

CFLAGS  = -Wall -g -I .

LD 	= gcc

LDFLAGS  = -Wall -g -L.

PUBFILES =  README  hungrymain.c  libPLN.a  libsnakes.a  lwp.h\
	    numbersmain.c  snakemain.c  snakes.h

TARGET =  pn-cs453@hornet:Given/asgn2

PROGS	= snakes nums hungry

SNAKEOBJS  = snakemain.o 

HUNGRYOBJS = hungrymain.o 

NUMOBJS    = numbersmain.o

OBJS	= $(SNAKEOBJS) $(HUNGRYOBJS) $(NUMOBJS) 

SRCS	= snakemain.c numbersmain.c

HDRS	= 

EXTRACLEAN = core $(PROGS)

all: 	$(PROGS)

allclean: clean
	@rm -f $(EXTRACLEAN)

clean:	
	rm -f $(OBJS) *~ TAGS

snakes: snakemain.o libPLN.a liblwp.so
	$(LD) $(LDFLAGS) -o snakes snakemain.o -L. -lncurses -lsnakes -llwp $(PLNICO)

hungry: hungrymain.o libPLN.a liblwp.so
	$(LD) $(LDFLAGS) -o hungry hungrymain.o -L. -lncurses -lsnakes -llwp $(PLNICO)

nums: numbersmain.o libPLN.a liblwp.so
	$(LD) $(LDFLAGS) -o nums numbersmain.o -L. -llwp $(PLNICO)

hungrymain.o: lwp.h snakes.h

snakemain.o: lwp.h snakes.h

numbermain.o: lwp.h

liblwp.so: lwp.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ lwp.c

libPLN.a: lwp.c
#	gcc -c lwp.c
#	ar r libPLN.a lwp.o
#	rm lwp.o

pub:
	scp $(PUBFILES) $(TARGET)

