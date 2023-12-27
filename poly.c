#include <stdint.h>
#include "cpucycles.h"
#include "params.h"
#include "reduce.h"
#include "poly.h"
#include "coding.h"
#include "cbd.h"
#include "inverse.h"
#if (FPTRU_N == 653)
#include "poly_mul_n653q/radix_ntt_n653.h"
#elif (FPTRU_N == 761)
#include "poly_mul_n761q/radix_ntt_n761.h"
#elif (FPTRU_N == 1277)
#include "poly_mul_n1277q/radix_ntt_n1277.h"
#endif

void poly_reduce(poly *a)
{
  for (int i = 0; i < FPTRU_N; ++i)
    a->coeffs[i] = barrett_reduce_int16_t(a->coeffs[i]);
}

void poly_freeze(poly *a)
{
  poly_reduce(a);
  for (int i = 0; i < FPTRU_N; ++i)
    a->coeffs[i] = fqcsubq(a->coeffs[i]);
}

void poly_fqcsubq(poly *a)
{
  for (int i = 0; i < FPTRU_N; ++i)
    a->coeffs[i] = fqcsubq(a->coeffs[i]);
}

void poly_add(poly *c, const poly *a, const poly *b)
{
  for (int i = 0; i < FPTRU_N; ++i)
    c->coeffs[i] = a->coeffs[i] + b->coeffs[i];
}

void poly_double(poly *b, const poly *a)
{
  for (int i = 0; i < FPTRU_N; ++i)
    b->coeffs[i] = a->coeffs[i] + a->coeffs[i];
}

void poly_inverse(poly *b, const poly *a)
{
  rq_inverse(b->coeffs, a->coeffs);
}

void poly_tomont(poly *a)
{
  const int16_t t = (MONT * MONT) % FPTRU_Q;
  for (int i = 0; i < FPTRU_N; ++i)
    a->coeffs[i] = fqmul_int16_t(a->coeffs[i], t);
}

void poly_frommont(poly *a)
{
  for (int i = 0; i < FPTRU_N; ++i)
    a->coeffs[i] = fqmul_int16_t(a->coeffs[i], 1);
}

void poly_sample(poly *a, const unsigned char buf[FPTRU_COIN_BYTES / 2])
{
#if (FPTRU_N == 653)
  cbd3(a, buf);
#elif (FPTRU_N == 761)
  cbd2(a, buf);
#elif (FPTRU_N == 1277)
  cbd2(a, buf);
#endif
}

void poly_encode_compress(poly *c,
                          const poly *sigma,
                          const unsigned char *msg)
{
  unsigned int i, j;
  int16_t mask;
  uint8_t mh[2 * FPTRU_MSGBYTES];
  uint8_t tmp;
  int16_t t;

  for (i = 0; i < FPTRU_MSGBYTES; i++)
  {
    tmp = msg[i] & 0xF;
    mh[2 * i] = encode_e8(tmp);

    tmp = (msg[i] >> 4) & 0xF;
    mh[2 * i + 1] = encode_e8(tmp);
  }

  for (i = 0; i < 2 * FPTRU_MSGBYTES; i++)
  {
    for (j = 0; j < 8; j++)
    {
      mask = -(int16_t)((mh[i] >> j) & 1);
      t = (int32_t)((sigma->coeffs[8 * i + j] << FPTRU_LOGQ2) + (FPTRU_Q >> 1)) / FPTRU_Q;
      t = t + (mask & (FPTRU_Q2 >> 1));
      c->coeffs[8 * i + j] = t & (FPTRU_Q2 - 1);
    }
  }
  for (j = 8 * i; j < FPTRU_N; j++)
  {
    t = (int32_t)((sigma->coeffs[j] << FPTRU_LOGQ2) + (FPTRU_Q >> 1)) / FPTRU_Q;
    c->coeffs[j] = t & (FPTRU_Q2 - 1);
  }
}

void poly_decode(unsigned char *msg,
                 const poly *c,
                 const poly *f)
{
  unsigned int i, j;
  poly mp;
  uint32_t tmp_mp[8];

  poly_mul_q2(&mp, c, f);

  for (i = 0; i < FPTRU_MSGBYTES; i++)
    msg[i] = 0;

  for (i = 0; i < 2 * FPTRU_MSGBYTES; i++)
  {
    for (j = 0; j < 8; j++)
    {
      tmp_mp[j] = (uint32_t)mp.coeffs[8 * i + j];
    }
    msg[i >> 1] |= decode_e8(tmp_mp) << ((i & 1) << 2);
  }
}
