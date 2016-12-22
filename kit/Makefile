
.PHONY=clean

kit.a:library/memory.o Makefile
	ar -r kit.a library/memory.o

library/memory.o:library/memory.cpp library/memory.h
	g++ -c -o library/memory.o library/memory.cpp

clean:
	-rm library/memory.o
	-rm kit.a