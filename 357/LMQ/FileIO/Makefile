# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -O3

CMPFILES = InsertSort.o 
CLEARFILES = InsertSort.o 

InsertSort: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o InsertSort

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o InsertSort

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

InsertSort.c: 
	touch InsertSort.c

