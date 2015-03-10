#ifndef __SCAN_H__
#define __SCAN_H__

#define DIMENTION 4
#define VEC_ADD_PRE(func) vector_add_ ## func ##D
#define userBinaryOperator(macro, ...) macro(__VA_ARGS__)


struct vec {
  double vec[DIMENTION];
};

void userBinaryOperator(VEC_ADD_PRE, DIMENTION) ( void *x1, void *x2);

#endif // __SCAN_H__
