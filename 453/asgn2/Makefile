#TESTING
PLNICO	= libPLN.a
LWPO = 

CC 	= gcc

CFLAGS  = -Wall -g -I .

LD 	= gcc

LDFLAGS  = -Wl,--allow-multiple-definition -Wall -g -L.

PUBFILES =  README  hungrymain.c  libPLN.a  libsnakes.a  lwp.h\
	    numbersmain.c  snakemain.c  snakes.h

TARGET =  pn-cs453@hornet:Given/asgn2

PROGS	= snakes nums hungry

SNAKEOBJS  = snakemain.o 

HUNGRYOBJS = hungrymain.o 

NUMOBJS    = numbersmain.o

OBJS	= $(SNAKEOBJS) $(HUNGRYOBJS) $(NUMOBJS) liblwp.so

SRCS	= snakemain.c numbersmain.c

HDRS	= 

EXTRACLEAN = core $(PROGS)

all: 	$(PROGS)

allclean: clean
	@rm -f $(EXTRACLEAN)

clean:	
	rm -f $(OBJS) *~ TAGS

snakes: snakemain.o libPLN.a liblwp.a
	$(LD) $(LDFLAGS) -o snakes snakemain.o $(LWPO) -L. -lncurses -lsnakes -llwp $(PLNICO)

hungry: hungrymain.o libPLN.a liblwp.a
	$(LD) $(LDFLAGS) -o hungry hungrymain.o $(LWPO) -L. -lncurses -lsnakes -llwp $(PLNICO)

nums: numbersmain.o libPLN.a liblwp.a
	$(LD) $(LDFLAGS) -o nums numbersmain.o $(LWPO) -L. -llwp $(PLNICO)

hungrymain.o: lwp.h snakes.h

snakemain.o: lwp.h snakes.h

numbermain.o: lwp.h

liblwp.so: lwp.o
	$(CC) $(CFLAGS) -fPIC -shared -o $@ lwp.c

lwp.o: lwp.c
	$(CC) $(CFLAGS) -c lwp.c

liblwp.a: lwp.o
	ar r $@ lwp.o

pub:
	scp $(PUBFILES) $(TARGET)

