#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "scan.h"
#include <math.h>

#define ubop(x1,x2) userBinaryOperator(VEC_ADD_PRE, DIMENTION)( (void *) (x1), (void *) (x2) );

void genericScan(void *X, int n, size_t l)
{
  int i, j;
  char *c = (char *)X;
  int tid;
  int offset = 1;
  int step = (int)ceil(log2(n));
  int nthreads;
#pragma omp parallel private(tid, i, j, nthreads, offset) shared(c, step, n, l) 
  {
    offset = 1;
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    int temp = n >> 1;
#ifdef _DEBUG
    if (tid == 0) printf("nthreads = %d\n", nthreads);
#endif
    for (i = step; i>0; i--) {
      #pragma omp barrier
      for (j = tid; j < temp; j += nthreads) {
        ubop(c+(offset*(2*j+2)-1)*l, c+(offset*(2*j+1)-1)*l);
      }
      offset *= 2;
      temp = temp >> 1;
    }
    temp  = 2;
    offset >>= 1;
    for (i = 1; i < step; i ++) {
      #pragma omp barrier
      offset >>= 1;
      for (j = tid; j < temp; j += nthreads) {
        int a = offset*(2*j+1)-1;
        int b = offset*(2*j)-1;
        if ( a < n &&  b < n)
          ubop(c+a*l, c+b*l);
      }
      temp *= 2;
    }
  }
}
