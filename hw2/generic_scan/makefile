all:scan run

mtime.o: mtime.cpp
	@g++ -c -Wall -O0 -g3 $< -o $@

scan.o: scan.c
	@g++ -c -Wall -O0 -g3 $< -o $@

scan:scan.o genscan.o mtime.o
	@g++ scan.o genscan.o mtime.o -o scan

run:scan
	@./scan 10

clean:
	@/bin/rm -rf *.o
