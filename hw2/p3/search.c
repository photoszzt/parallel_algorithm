#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "bsearch.h"
#include <set>
#include <stdlib.h>
#include <algorithm>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <size_of_array>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int arr_size = atoi(argv[1]);
  MPI_Comm comm = MPI_COMM_WORLD;
  int * arr = (int*) malloc(arr_size*sizeof(int));
  int * keys = (int*) malloc(sizeof(int)*NUM_KEYS);
  int * pos = (int*) malloc(sizeof(int)*NUM_KEYS);
  MPI_Init(NULL, NULL);
  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  if (rank == 0) {
    srand(time(NULL));
    for (int i = 0; i < NUM_KEYS; i++) {
      keys[i] = rand() % arr_size;
    }

    std::set<int> numbers_gen;
    for (int i = 0; i < arr_size; i++) {
      // TODO: Here I'm not sure whether it should be bounded by arr_size
      int new_val = rand() % arr_size;
      while (numbers_gen.find(new_val) != numbers_gen.end())
        new_val = rand() % arr_size;
      arr[i] = new_val;
      numbers_gen.insert(new_val);
    }
    std::sort(arr, arr+arr_size);
  }
  int len = arr_size / size;
  int * sub_arr = (int*) malloc(sizeof(int)*len);
  MPI_Scatter(arr, len, MPI_INT, sub_arr, len, MPI_INT, 0, comm);
  MPI_Bcast(keys, NUM_KEYS, MPI_INT, 0, comm);
#if 1
  init_and_bsearch(comm, keys, NUM_KEYS, sub_arr, len, NUM_THREADS, rank, &pos);
#endif
  MPI_Finalize();
  free(arr);
  free(keys);
  free(pos);
  return 0;
}
