#include <mpi.h>
#include <omp.h>
#include <time.h>
#include <string.h>

//#define _MPI_SEND

//#define _DEBUG

int init_and_bsearch(MPI_Comm comm, int* keys, int num_keys, int* arr, int arr_size, int num_ts, int rank, int** pos) {
  int low = arr_size*rank;
  int high = low+arr_size - 1;
  int tid; 
  int i;
  int nthreads;
  int send_buf[2][2];
  int recv_buf[2];
  for (i = 0; i < 2; i++) {
    memset(send_buf[i], 0, sizeof(int)*2);
  }
  memset(recv_buf, 0, sizeof(int)*2);
  printf("num_keys = %d\n", num_keys);
  for (i = 0; i < num_keys; i++ ) {
    int k = keys[i];
    if (k < arr[high] && k > arr[low]) {
      int position = low;

      if (arr_size - 1 <= num_ts) {
        // each thread deals with size=1 interval
#pragma omp parallel shared(keys, num_keys, arr, arr_size, low, high, k, position, i) private(tid,nthreads) num_threads(num_ts) 
        {
          tid = omp_get_thread_num();
          nthreads = omp_get_num_threads();

          if ((tid < arr_size - 1) && arr[low + tid] <= k && arr[low + tid + 1]) {
            position = low;
          }
#pragma omp barrier
        }
      } else {
        int len = (arr_size - 1) / num_ts + 1;
        position = low;

        while (len != 1) {

        // each thread deals with a interval, need binary search
#pragma omp parallel shared(keys, num_keys, arr, arr_size, low, high, k, position, i, len) private(tid,nthreads) num_threads(num_ts) 
          {
            tid = omp_get_thread_num();
            nthreads = omp_get_num_threads();

#ifdef _DEBUG
            printf("len = %d, tid = %d, nthreads = %d\n", len, tid, nthreads);
#endif

            int left = low + tid * len;
            int right = (left + len - 1) > high ? high : (left + len - 1);

            if (arr[left] == k) {
              position = left;
              len = 1;
            } else if (arr[right] == k) {
              position = right;
              len = 1;
            } else if (arr[left] < k && arr[right] > k) {
              len = (len - 1) / num_ts + 1;
              low = left;
              high = right;
              position = low;
            }
#pragma omp barrier
          }

        }
      }

      send_buf[0][0] = position;
      send_buf[0][1] = i;

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      MPI_Isend(send_buf[0], 2, MPI_INT, 0, 2, comm, &req);
      MPI_Wait(&req, &status);
#endif
    } else if ((k == arr[high]) || (k == arr[low])) {
      if (k == arr[high])
        send_buf[1][0] = high;
      else
        send_buf[1][0] = low;
      send_buf[1][1] = i;

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      MPI_Isend(send_buf[1], 2, MPI_INT, 0, 1, comm, &req);
      MPI_Wait(&req, &status);
#endif
    } 
    if (rank == 0) {

      MPI_Request req;
      MPI_Status status;
#ifdef _MPI_SEND
      MPI_Irecv(recv_buf, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &req);
      MPI_Wait(&req, &status);
#endif
      (*pos)[recv_buf[1]] = recv_buf[0];
    }
  }
}
