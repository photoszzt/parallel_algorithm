all:scan

scan.o: scan.c
	@g++ -g -c -o scan.o scan.c

genscan.o: genscan.c
	@g++ -g -c -fopenmp -o genscan.o genscan.c

scan:scan.o genscan.o mtime.o
	@g++ -fopenmp -g scan.o genscan.o mtime.o -o scan

run:scan
	@./scan 10

mtime.o: mtime.cpp
	@g++ -c -Wall -O0 -g3 $< -o $@

PHONY: clean

clean:
	@/bin/rm -rf *.o
