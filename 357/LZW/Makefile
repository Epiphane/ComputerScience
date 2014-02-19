# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -g -O3

CMPFILES = CodeSet.o Compress.o LZWCmp.o SmartAlloc.o 
CLEARFILES = CodeSet.o Compress.o LZWCmp.o SmartAlloc.o 

Compress: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o Compress

Elliot: CodeSetE.c CompressE.c LZWCmpE.c SmartAlloc.c
	gcc -O3 -w CodeSetE.c CompressE.c LZWCmpE.c SmartAlloc.c -o Elliot

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o Compress

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

CodeSet.c: SmartAlloc.h CodeSet.h 
	touch CodeSet.c

Compress.c: LZWCmp.h SmartAlloc.h 
	touch Compress.c

LZWCmp.c: LZWCmp.h SmartAlloc.h 
	touch LZWCmp.c

SmartAlloc.c: 
	touch SmartAlloc.c

