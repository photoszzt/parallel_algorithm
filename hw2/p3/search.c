#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include "bsearch.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <size_of_array>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int arr_size = atoi(argv[1]);
  MPI_Comm comm = MPI_COMM_WORLD;
  int * keys = (int*) malloc(sizeof(int)*NUM_KEYS);
//  int * pos = (int*) malloc(sizeof(int)*NUM_KEYS);
  MPI_Init(NULL, NULL);
  int rank, size;
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  /** 1. Key generation. */
  MPI_Barrier(comm);

  int key_per_proc = NUM_KEYS / size;
  int * key_chunk = (int*) malloc(sizeof(int) * key_per_proc);
  init_keys(comm, key_chunk, key_per_proc, arr_size, rank);
  MPI_Barrier(comm);

  MPI_Gather(key_chunk, key_per_proc, MPI_INT,
               keys, key_per_proc, MPI_INT, 0, comm);
  MPI_Barrier(comm);
  free(key_chunk);

#ifdef _DEBUG
  if (rank == 0) {
    printf("Keys:\n");
    for (int i = 0; i < NUM_KEYS; i++) {
      printf("%d ", keys[i]);
    }
    printf("\n");
  }
#endif

printf("Key prepared.... \n");

  MPI_Bcast(keys, NUM_KEYS, MPI_INT, 0, comm);

  MPI_Barrier(comm);

  /** 2. Array initialization. */
  int len = arr_size / size;

  int* sub_arr = (int *) malloc(len * sizeof(int));
  init(comm, sub_arr, len, len * rank);
  MPI_Barrier(comm);

printf("Arr prepared.... \n");

  /** 3. Run binary search. */
  double start, end;
//  if (rank == 0) {
    start = MPI_Wtime();
//  }

printf("Start running.... \n");

  bsearch(comm, keys, NUM_KEYS, sub_arr, len, NUM_THREADS, rank);
  //bsearch(comm, keys, NUM_KEYS, sub_arr, len, NUM_THREADS, rank, &pos);

printf("Rank %d finished.... \n", rank);

  MPI_Barrier(comm);

printf("Run finished.... \n");

//  if (rank == 0) {
    end = MPI_Wtime();
    printf("%d Finished. Elapsed time is %f seconds\n", rank, end - start); 
//  }

  MPI_Finalize();
  free(keys);
//  free(pos);
  return 0;
}
