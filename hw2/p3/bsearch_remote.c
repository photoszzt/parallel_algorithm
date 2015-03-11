#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include "bsearch.h"
#include <set>
#include <stdlib.h>
#include <algorithm>

//#define _MPI_SEND

#define _PRINT_OUT
//#define _DEBUG

void init_keys(MPI_Comm comm, int* key_chunk, int key_per_proc, int arr_size, 
    int rank) {
  srand(time(NULL) + rank);
  for (int i = 0; i < key_per_proc; i++) {
//    key_chunk[i] = rand() % arr_size;
    key_chunk[i] = i + key_per_proc * rank;
  }
}

void init(MPI_Comm comm, int* sub_arr, int arr_size, int low) {
  srand(time(NULL));
  std::set<int> numbers_gen;
#if 0
  for (int i = 0; i < arr_size; i++) {
    int new_val = rand() % arr_size + low;
    while (numbers_gen.find(new_val) != numbers_gen.end())
      new_val = rand() % arr_size + low;
    sub_arr[i] = new_val;
    numbers_gen.insert(new_val);
  } 
  std::sort(sub_arr, sub_arr+arr_size);
#else
  for (int i = 0; i < arr_size; i++) {
    sub_arr[i] = low + i;
  }
#endif
}

void bsearch(MPI_Comm comm, int* keys, int num_keys, int* sub_arr, int arr_size, int num_ts, int rank) {

  int low_pos = arr_size * rank;
  int high_pos = low_pos + arr_size - 1;
  int low = 0, high = arr_size - 1;
  int tid; 
  int i;
  int nthreads;
  int send_buf[2][2];
  int recv_buf[2];

#ifdef _DEBUG
  printf("Rank = %d\n\tnum_keys = %d, Keys: %d %d %d %d\n\tarr_size = %d, "
      "arrs: %d\n", 
      rank, num_keys, keys[0], keys[1], keys[2], keys[3], arr_size, sub_arr[0]);
#endif

  for (i = 0; i < 2; i++) {
    memset(send_buf[i], 0, sizeof(int)*2);
  }
  memset(recv_buf, 0, sizeof(int)*2);
  for (i = 0; i < num_keys; i++ ) {

    low = 0, high = arr_size - 1;
    int position = low;
    bool sendout = true;
    int k = keys[i];

    if (k < sub_arr[high] && k > sub_arr[low]) {

      if (arr_size - 1 <= num_ts) {
        // each thread deals with size=1 interval
#pragma omp parallel shared(keys, num_keys, sub_arr, arr_size, low, high, k, position, i) private(tid,nthreads) num_threads(num_ts) 
        {
          tid = omp_get_thread_num();
          nthreads = omp_get_num_threads();

          if ((tid < arr_size - 1) && sub_arr[low + tid] <= k && sub_arr[low + tid + 1]) {
            position = low;
          }
#pragma omp barrier
        }
      } else {
#if 1
        int len = (arr_size - 1) / num_ts + 1;
        position = low;









        while (len > 1 && high - low -1 > num_ts) {





/** FIX **/
#if 1
      int new_arr_size = high - low;

      if (new_arr_size - 1 <= num_ts) {
        // each thread deals with size=1 interval
#pragma omp parallel shared(keys, num_keys, sub_arr, new_arr_size, low, high, k, position, i) private(tid,nthreads) num_threads(num_ts) 
        {
          tid = omp_get_thread_num();
          nthreads = omp_get_num_threads();

          if ((tid < new_arr_size - 1) && sub_arr[low + tid] <= k && sub_arr[low + tid + 1]) {
            position = low;
          }
#pragma omp barrier
        }
break;
      }
#endif






        // each thread deals with a interval, need binary search
#pragma omp parallel shared(keys, num_keys, sub_arr, arr_size, low, high, k, position, i, len) private(tid,nthreads) num_threads(num_ts) 
          {
            tid = omp_get_thread_num();
            nthreads = omp_get_num_threads();

#ifdef _DEBUG
            printf("len = %d, tid = %d, nthreads = %d\n", len, tid, nthreads);
#endif

            int left = (low + tid * len) > high ? high : (low + tid * len);
            int right = (left + len - 1) > high ? high : (left + len - 1);




#if 1
if (left < arr_size && right < arr_size) { 
            if (sub_arr[left] == k) {
              position = left;
              len = 1;
            } else if (sub_arr[right] == k) {
              position = right;
              len = 1;
            }
#if 0
    else {len = 1;
              low = left;
              high = right;
              position = low;}
#endif
#if 1
 else if (sub_arr[left] < k && sub_arr[right] > k) {
              low = left;
              high = right;
              int len_new = (high - low - 1) / nthreads + 1;
if (len_new < len) {len = len_new;}
else {len = 1;}
printf("Rank %d, left %d, right %d, len %d\n", rank, left, right, len);
//len = 1;
              position = low;
            }
#endif
} else { 
len = 1;
}
#endif
//len = 1;



#pragma omp barrier


          }

        }
printf(">>>>>>>>>>>>>>>>>>>>>>");

#endif
      }

    } else if ((k == sub_arr[high]) || (k == sub_arr[low])) {
      if (k == sub_arr[high])
        send_buf[1][0] = high + low_pos;
      else
        send_buf[1][0] = low + low_pos;
      send_buf[1][1] = i;

    } 
  }
}
