# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -O3 -g

CMPFILES = AltStdio2xUngetc.o AltStdio2xUngetcTest.o 
CLEARFILES = AltStdio2xUngetc.o AltStdio2xUngetcTest.o 

DoubleUngetc: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o DoubleUngetc

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o DoubleUngetc

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

AltStdio2xUngetc.c: AltStdio.h 
	touch AltStdio2xUngetc.c

AltStdio2xUngetcTest.c: AltStdio.h 
	touch AltStdio2xUngetcTest.c

