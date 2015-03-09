#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <time.h>
#include <set>
#include<stdlib.h>

int init(MPI_Comm comm, int* arr, int arr_size, int low) {
  srand(time(NULL));
  std::set<int> numbers_gen;
  for (int i = 0; i < arr_size; i++) {
    int new_val = rand() % arr_size + low;
    while (numbers_gen.find(new_val) != numbers_gen.end())
      new_val = rand() % arr_size + low;
    arr[i] = new_val;
    numbers_gen.insert(new_val);
  } 
  sort(arr, arr+arr_size);
}

int init_and_bsearch(MPI_Comm comm, int* keys, int num_keys, int* arr, int arr_size, int num_ts, int rank, int** pos) {
  int low = arr_size*rank;
  int high = low+arr_size;
  init(comm, arr, arr_size, low);
  int tid; 
  int i;
  int nthreads;
  int send_buf[2][2];
  int recv_buf[2];
  for (i = 0; i < 2; i++) {
    memset(send_buf[i], 0, sizeof(int)*2);
  }
  memset(recv_buf, 0, sizeof(int)*2);
  for (i = 0; i < num_keys; i++ ) {
    int k = keys[i];
    if (k < arr[high-1] && k > arr[low]) {
      int position = low;
#pragma omp parallel shared(keys, num_keys, arr, arr_size, len, low, high, k, position) private(tid, i, nthreads) num_threads(num_ts) 
      {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        int len = arr_size / num_ts;
        int chunk_low = tid*len;
        int chunk_high = chunk_low+len;
        while (len != 1 && arr[low+chunk_low] < k && arr[low+chunk_high-1] > k) {
          len = len / num_ts;
          low += chunk_low;
          high = low+len;
          chunk_low = tid*len;
          chunk_high = chunk_low+len;
          position = low;
        }
      }
      send_buf[0][0] = position;
      send_buf[0][1] = i;
      MPI_Request req;
      MPI_Status status;
      MPI_Isend(send_buf[0], 2, MPI_INT, 0, 2, comm, &req);
      MPI_Wait(&req, &status);
    } else if ((k == arr[high-1]) || (k == arr[low])) {
      if (k == arr[high-1])
        send_buf[1][0] = high-1;
      else
        send_buf[1][0] = low;
      send_buf[1][1] = i;
      MPI_Request req;
      MPI_Status status;
      MPI_Isend(send_buf[1], 2, MPI_INT, 0, 1, comm, &req);
      MPI_Wait(&req, &status);
    } 
    if (rank == 0) {
      MPI_Request req;
      MPI_Status status;
      MPI_Irecv(recv_buf, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &req);
      MPI_Wait(&req, &status);
      (*pos)[recv_buf[1]] = recv_buf[0];
    }
  }
}
