#ifndef __SCAN_H__
#define __SCAN_H__

#define DIMENTION 4

struct vec {
  double vec[DIMENTION];
};

void genericScan(void *X, int n, size_t l);
void userBinaryOperator( void *x1, void *x2);
#endif
