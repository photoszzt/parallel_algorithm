#ifndef __BSEARCH_H__
#define __BSEARCH_H__

//#define NUM_KEYS (0x1 << 20)
#define NUM_KEYS (0x1 << 5)
#define NUM_THREADS 6

int init_and_bsearch(MPI_Comm comm, int* keys, int num_keys, int* arr, int arr_size, int num_ts, int rank, int** pos);

#endif // __BSEARCH_H__
