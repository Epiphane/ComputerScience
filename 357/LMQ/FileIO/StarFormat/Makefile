# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -O3 -g

CMPFILES = AltStdioStarFmt.o AltStdioStarFmtTest.o 
CLEARFILES = AltStdioStarFmt.o AltStdioStarFmtTest.o 

StarFmt: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o StarFmt

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o StarFmt

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

AltStdioStarFmt.c: AltStdio.h 
	touch AltStdioStarFmt.c

AltStdioStarFmtTest.c: AltStdio.h 
	touch AltStdioStarFmtTest.c

