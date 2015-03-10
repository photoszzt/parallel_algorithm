#ifndef __BSEARCH_H__
#define __BSEARCH_H__

#define NUM_KEYS (1 << 20)
//#define NUM_KEYS (1 << 4)
#define NUM_THREADS 6

void init_keys(MPI_Comm comm, int* key_chunk, int key_per_proc, int arr_size, 
    int rank);

void init(MPI_Comm comm, int* sub_arr, int arr_size, int low);

void bsearch(MPI_Comm comm, int* keys, int num_keys, int* sub_arr, int arr_size, int num_ts, int rank);

#endif // __BSEARCH_H__
