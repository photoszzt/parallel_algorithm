all:seqscan run

mtime.o: mtime.cpp
	@g++ -c -Wall -O0 -g3 $< -o $@

genscan.o: genscan.c
	@g++ -c -Wall -O0 -g3 $< -o $@

scan.o: scan.c
	@g++ -c -Wall -O0 -g3 $< -o $@

seqscan:scan.o genscan.o mtime.o
	@g++ scan.o genscan.o mtime.o -o seqscan

run:seqscan
	@./seqscan 10

clean:
	@/bin/rm -rf *.o scan
