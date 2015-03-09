#include <stdio.h>
#include <stdlib.h>
#include "mtime.h"

void genericScan(void *X, int n, size_t l);

// the use must define this function to use genericScan
void userBinaryOperator( void *x1, void *x2){
	int *a = (int *) x1;
	int *b = (int *) x2;
  
  *a += *b;
}


int main( int argc, char **argv){	
	
  // problem setup
  int n=10;
  if(argc>1) n=atoi(argv[1]);
  int *x = (int *) malloc( sizeof(int)*n) ;
  for(int i=0;i<n;i++) x[i] = i%2;
  
  // print input
  for(int i=0;i<n;i++) printf("%d ",x[i]); 	printf("\n");
  
  // scan
  int64_t start = GetTimeMius64(); 
  genericScan((void*) x,n,sizeof(int));
  int64_t duration = GetTimeMiusFrom(start);
  printf("\nComputation time: %lld microseconds\n\n", (long long) duration);

  //print output
  for(int i=0;i<n;i++) printf("%d ",x[i]); 	printf("\n");
  
  // clean up
  free(x);
  return 0;
}
	




	


	



