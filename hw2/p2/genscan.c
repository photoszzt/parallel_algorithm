#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "scan.h"
#include <math.h>

#define ubop(x1,x2) userBinaryOperator(VEC_ADD_PRE, DIMENTION)( (void *) (x1), (void *) (x2) );

void genericScan(void *X, int n, size_t l)
{
  int i;
  char *c = (char *)X;
  int tid;
  int offset = 1;
  int step = (int)ceil(log2(n));
  printf("step: %d\n", step);
  int nthreads;
#pragma omp parallel private(tid, i, nthreads, offset) shared(c, step, n, l) 
  {
    offset = 1;
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("num threads: %d\n", nthreads);
    }
    int temp = n >> 1;
#pragma omp parallel for 
    for (i = step; i>0; i--) {
      printf("First for loop: %d\n", tid);
      if (tid < temp) {
        printf("access: %d, %d\n", offset*(2*tid+1)-1, offset*(2*tid+2)-1);
        ubop(c+(offset*(2*tid+2)-1)*l, c+(offset*(2*tid+1)-1)*l);
        printf("%f, %f\n", ((double*)c)[(offset*(2*tid+2)-1)], ((double*)c)[(offset*(2*tid+1)-1)]);
      }
      printf("offset, temp: %d, %d\n", offset, temp);
      offset *= 2;
      temp = temp >> 1;
    }
/* #pragma omp barrier */
/*     temp  = 1; */
/*     offset >>= 1; */
/* #pragma omp parallel for */
/*     for (i = 1; i < step; i ++) { */
/*       offset >>= 1; */
/*       printf("Second for loop: %d\n", tid); */
/*       printf("offset, temp: %d, %d\n", offset, temp); */
/*       if (tid <= temp && tid > 0) { */
/*         printf("access: %d, %d\n", offset*(2*tid+1)-1, offset*(2*(tid-1)+2)-1); */
/*         if (offset*(2*tid+1)-1 < n && offset*(2*(tid-1)+2)-1 < n) */
/*           ubop(c+(offset*(2*tid+1)-1)*l, c+(offset*(2*(tid-1)+2)-1)*l); */
/*       } */
/*       temp *= 2; */

/*     } */
  }
}


