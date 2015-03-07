#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdint.h>
#include "bsearch.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <size_of_array>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  uint32_t size_of_arr = atoi(argv[1]);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Init(&argc, &argv);
  MPI_Finalize();
  return 0;
}
