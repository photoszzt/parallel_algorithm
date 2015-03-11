#include <stdlib.h>
#include "scan.h"

#define ubop(x1,x2) userBinaryOperator( (void *) (x1), (void *) (x2) );

void genericScan(void *X, int n, size_t l)
{
	int i;
	char *c = (char *)X;

	for (i=1;i<n;i++)	
		ubop( (void *)(c+i*l),  (void *)(c+l*(i-1)) ); 
}


