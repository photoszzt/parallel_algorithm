#include <mpi.h>
#include <omp.h>

int bsearch(MPI_Comm comm, int* keys, int num_keys, int* arr, int arr_size, int num_threads) {
  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  int len = arr_size/size;
  int low = len*rank;
  int high = low+len;
  
}
