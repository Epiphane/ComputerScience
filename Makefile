all:
	g++ -O2 -g decode.cc -o decode

run:
	./decode -f input.sim

clean:
	rm -f *.o ./decode
