#include <mpi.h>
#include <omp.h>
#include <stdint.h>

int bsearch(MPI_Comm comm, uint32_t* keys, uint32_t num_keys, uint32_t* arr, uint32_t arr_size, uint32_t num_threads) {
  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  int len = arr_size/size;
  int low = len*rank;
  int high = low+len;
  
}
