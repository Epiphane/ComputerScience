# Autmatically generated Makefile
# tester.py by Thomas Steinke

CPP = gcc
CPPFLAGS = -O3

CMPFILES = DictionarySort.o 
CLEARFILES = DictionarySort.o 

DictionarySort: $(CMPFILES)
	$(CPP) $(CPPFLAGS) $(CMPFILES) -o DictionarySort

debug: $(CMPFILES)
	$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o DictionarySort

clear:
	rm $(CLEARFILES)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o $@

DictionarySort.c: 
	touch DictionarySort.c

