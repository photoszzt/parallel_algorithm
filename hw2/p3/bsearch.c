#include <mpi.h>
#include <omp.h>
#include <stdint.h>

int bsearch(MPI_Comm comm, uint32_t num_keys, uint32_t num_threads) {
  uint32_t rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  
}
