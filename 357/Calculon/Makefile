# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -g -O3

CMPFILES = Calculon.o RunSuite.o Parse.o Array.o

Calculon: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o Calculon

clear:
	rm $(CMPFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.c: %.h Calculon.h 
	touch $@
