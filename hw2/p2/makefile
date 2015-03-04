all:scan

scan.o: scan.c
	@g++ -g -c -o scan.o scan.c

genscan.o: genscan.c
	@g++ -g -c -fopenmp -o genscan.o genscan.c

scan:scan.o genscan.o
	@g++ -fopenmp -g scan.o genscan.o -o scan

run:scan
	@./scan 10

PHONY: clean

clean:
	@/bin/rm -rf *.o
