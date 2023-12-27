#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

#if (FPTRU_Q == 4091)
#define MONT 80
#define QINV 41779
#define V_BARRETT 16404
#elif (FPTRU_Q == 4591)
#define MONT 1262
#define QINV 15631
#define V_BARRETT 29235
#elif (FPTRU_Q == 4621)
#define MONT 842
#define QINV 36037
#define V_BARRETT 29045
#elif (FPTRU_Q == 7879)
#define MONT 2504
#define QINV 17143
#define V_BARRETT 17035
#endif

int16_t montgomery_reduce_int16_t(int32_t a);
int16_t barrett_reduce_int16_t(int16_t a);
int16_t fqcsubq(int16_t a);
int16_t fqmul_int16_t(int16_t a, int16_t b);

#endif
