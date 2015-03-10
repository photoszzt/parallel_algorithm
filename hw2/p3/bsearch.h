#ifndef __BSEARCH_H__
#define __BSEARCH_H__

#define NUM_KEYS (0x1 << 20)
//#define NUM_KEYS (0x1 << 2)
#define NUM_THREADS 6

int init_keys(MPI_Comm comm, int* key_chunk, int key_per_proc, int arr_size, 
    int rank);

int init(MPI_Comm comm, int* sub_arr, int arr_size, int low);

int bsearch(MPI_Comm comm, int* keys, int num_keys, int* sub_arr, int arr_size, int num_ts, int rank, int** pos);

#endif // __BSEARCH_H__
