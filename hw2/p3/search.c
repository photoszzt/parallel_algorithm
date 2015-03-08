#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <set>
#include "bsearch.h"


int init_arr(MPI_Comm comm, int* arr, int arr_size) {
  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  int len = arr_size / size;
  int low = len*rank;
  int high = low + len;
  srand(time(NULL));
  std::set<int> numbers_gen;
  for (int i = low; i < high; i++) {
    int new_val = rand() % len + low;
    while (numbers_gen.find(new_val) != numbers_gen.end())
      new_val = rand() % len + low;
    arr[i] = arr[i];
  } 
  sort(arr+low, arr+high);
}

int main(int argc, char** argv) {
 if (argc != 2) {
    fprintf(stderr, "Usage: %s <size_of_array>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int arr_size = atoi(argv[1]);
  MPI_Comm comm = MPI_COMM_WORLD;
  int * arr = (int*) malloc(size_of_arr*sizeof(int));
  MPI_Init(&argc, &argv);
  init_arr(comm, arr, arr_size);
  bsearch(comm, keys, NUM_KEYS, arr, arr_size, NUM_THREADS);
  MPI_Finalize();
  return 0;
}
