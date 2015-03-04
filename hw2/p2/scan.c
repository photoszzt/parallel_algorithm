#include <stdio.h>
#include <stdlib.h>
#include "scan.h"

void genericScan(void *X, int n, size_t l);

// the use must define this function to use genericScan
void userBinaryOperator(VEC_ADD_PRE, DIMENTION) ( void *x1, void *x2) {
  vec* a = (vec*) x1;
  vec* b = (vec*) x2;
  int i = 0;
  for (i = 0; i < DIMENTION; i++) {
    a->vec[i] += b->vec[i];
  }
}


int main( int argc, char **argv){	
	
	// problem setup
	int n=8;
	if(argc>1) n=atoi(argv[1]);
	vec *x = (vec *) malloc( sizeof(vec)*n) ;
	for(int i=0;i<n;i++) {
          for (int j=0; j < DIMENTION; j++)
            x[i].vec[j] = i%2;
        }

	// print input
	for(int i=0;i<n;i++)
          if (DIMENTION > 1) {
            printf("(");
            for (int j=0; j<DIMENTION-1; j++) {
              printf("%.3f,", x[i].vec[j]);
            }
            printf("%.3f)", x[i].vec[DIMENTION-1]);
          } else {
            printf("%.3f ",x[i].vec[0]);
          }
        printf("\n");

	// scan
	genericScan((void*) x,n,sizeof(vec));

	//print output
        for(int i=0;i<n;i++)
          if (DIMENTION > 1) {
            printf("(");
            for (int j=0; j<DIMENTION-1; j++) {
              printf("%.3f,", x[i].vec[j]);
            }
            printf("%.3f)", x[i].vec[DIMENTION-1]);
          } else {
            printf("%.3f ",x[i].vec[0]);
          }
        printf("\n");

	// clean up
	free(x);
	return 0;
}
	




	


	



