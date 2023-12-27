#include <stdint.h>
#include <math.h>
#include "params.h"
#include "randombytes.h"
#include "reduce.h"

#if (FPTRU_N == 653)
#include "poly_mul_n653q/radix_ntt_n653.h"
#elif (FPTRU_N == 761)
#include "poly_mul_n761q/radix_ntt_n761.h"
#elif (FPTRU_N == 1277)
#include "poly_mul_n1277q/radix_ntt_n1277.h"
#endif

int16_t montgomery_reduce_int16_t(int32_t a)
{
  int32_t t;
  int16_t u;

  u = a * QINV;
  t = (int32_t)u * FPTRU_Q;
  t = a - t;
  t >>= 16;
  return t;
}

int16_t barrett_reduce_int16_t(int16_t a)
{
  int32_t t;

#if (FPTRU_LOGQ == 12)
  t = V_BARRETT * a;
  t >>= 26;
  t *= FPTRU_Q;
#elif (FPTRU_LOGQ == 13)
  t = V_BARRETT * a;
  t >>= 27;
  t *= FPTRU_Q;
#endif

  return a - t;
}

int16_t fqcsubq(int16_t a)
{
  a += (a >> 15) & FPTRU_Q;
  a -= FPTRU_Q;
  a += (a >> 15) & FPTRU_Q;
  return a;
}

int16_t fqmul_int16_t(int16_t a, int16_t b)
{
  return montgomery_reduce_int16_t((int32_t)a * b);
}

