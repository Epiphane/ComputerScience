# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -O3

CMPFILES = StdArgs.o 
CLEARFILES = StdArgs.o 

StdArgs: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o StdArgs

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o StdArgs

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

StdArgs.c: 
	touch StdArgs.c

