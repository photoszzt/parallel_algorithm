all: search

bsearch.o: bsearch.c
	mpic++ -fopenmp -c -o bsearch.o bsearch.c

search.o: search.c
	mpic++ -fopenmp -c -o search.o search.c

search: search.o bsearch.o
	mpic++ -fopenmp -o search search.o bsearch.o

.PHONY: clean

clean:
	rm -rf *.o search
