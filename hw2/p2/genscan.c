#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "scan.h"
#include <math.h>

//#define _DEBUG

#define ubop(x1,x2) userBinaryOperator(VEC_ADD_PRE, DIMENTION)( (void *) (x1), (void *) (x2) );

void genericScan(void *X, int n, size_t l)
{
  int i, j;
  char *c = (char *)X;
  int tid;
  int offset;
  int nthreads;
  int step = (int)ceil(log2(n));
#pragma omp parallel private(tid, i, j, nthreads, offset) shared(c, n, l, step) 
  {
    offset = 1;
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    bool done=false;
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
#ifdef _DEBUG
    printf(" tid=%d,off=%d,step=%d ", tid, offset, step);
#endif
    for (i = 1; i < step; i ++) {
      #pragma omp barrier
      offset >>= 1;
      j=tid;
      done=false;
      while (!done && j < temp) {
      int a = offset*(2*j+1)-1;
        int b = offset*(2*j)-1;
#ifdef _DEBUG
        printf(" tid=%d,off=%d,a=%d,b=%d,j=%d,temp=%d ", tid, offset, a, b, j, temp);
#endif
        if (a >= n)
          done=true;
        if ( a < n && b > 0)
          ubop(c+a*l, c+b*l);
        j+=nthreads;
    } 
      temp *= 2;
      
    }
  }
}
