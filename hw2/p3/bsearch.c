#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include "bsearch.h"
#include <set>
#include <stdlib.h>
#include <algorithm>

#define _MPI_SEND

//#define _DEBUG

int init(MPI_Comm comm, int* sub_arr, int arr_size, int low) {
  srand(time(NULL));
  std::set<int> numbers_gen;
  for (int i = 0; i < arr_size; i++) {
    int new_val = rand() % arr_size + low;
    while (numbers_gen.find(new_val) != numbers_gen.end())
      new_val = rand() % arr_size + low;
    sub_arr[i] = new_val;
    numbers_gen.insert(new_val);
  } 
  std::sort(sub_arr, sub_arr+arr_size);
}

int bsearch(MPI_Comm comm, int* keys, int num_keys, int* sub_arr, int arr_size, int num_ts, int rank, int** pos) {

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
        int len = (arr_size - 1) / num_ts + 1;
        position = low;

        while (len != 1) {

        // each thread deals with a interval, need binary search
#pragma omp parallel shared(keys, num_keys, sub_arr, arr_size, low, high, k, position, i, len) private(tid,nthreads) num_threads(num_ts) 
          {
            tid = omp_get_thread_num();
            nthreads = omp_get_num_threads();

#ifdef _DEBUG
            printf("len = %d, tid = %d, nthreads = %d\n", len, tid, nthreads);
#endif

            int left = low + tid * len;
            int right = (left + len - 1) > high ? high : (left + len - 1);

            if (sub_arr[left] == k) {
              position = left;
              len = 1;
            } else if (sub_arr[right] == k) {
              position = right;
              len = 1;
            } else if (sub_arr[left] < k && sub_arr[right] > k) {
              len = (len - 1) / num_ts + 1;
              low = left;
              high = right;
              position = low;
            }
#pragma omp barrier
          }

        }
      }

      send_buf[0][0] = position + low_pos;
      send_buf[0][1] = i;

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      if (rank == 0) {
        sendout = false;
      } else {
#ifdef _DEBUG
        printf("++++ Send: rank = %d, k = %d, i = %d\n", rank, k, i);
#endif
        MPI_Isend(send_buf[0], 2, MPI_INT, 0, 2, comm, &req);
        MPI_Wait(&req, &status);
      }
#endif
    } else if ((k == sub_arr[high]) || (k == sub_arr[low])) {
      if (k == sub_arr[high])
        send_buf[1][0] = high + low_pos;
      else
        send_buf[1][0] = low + low_pos;
      send_buf[1][1] = i;

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      if (rank == 0) {
        sendout = false;
      } else {
#ifdef _DEBUG
        printf("++++++++ Send: rank = %d, k = %d, i = %d\n", rank, k, i);
#endif
        MPI_Isend(send_buf[1], 2, MPI_INT, 0, 1, comm, &req);
        MPI_Wait(&req, &status);
      }
#endif
    } 
    if (rank == 0) {

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      if (sendout) {
        MPI_Irecv(recv_buf, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &req);
        MPI_Wait(&req, &status);
#ifdef _DEBUG
        printf("--- Recv: rank = %d, k = %d, i = %d\n", 
            rank, recv_buf[0], recv_buf[1]);
#endif

      } else {
        recv_buf[0] = position + low_pos;
        recv_buf[1] = i;
      }
#endif
      (*pos)[recv_buf[1]] = recv_buf[0];

      printf("#%d: key = %d, pos = %d\n", 
          recv_buf[1], keys[recv_buf[1]], recv_buf[0]);
    }

  }
}
