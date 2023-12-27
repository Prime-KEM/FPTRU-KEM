#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

typedef struct
{
  int16_t coeffs[FPTRU_N];
} poly;

#if (FPTRU_N == 653)
#define poly_mul_q1 poly_radix_ntt_n653_q1
#define poly_mul_q2 poly_radix_ntt_n653_q2
#elif (FPTRU_N == 761)
#define poly_mul_q1 poly_radix_ntt_n761_q1
#define poly_mul_q2 poly_radix_ntt_n761_q2
#elif (FPTRU_N == 1277)
#define poly_mul_q1 poly_radix_ntt_n1277_q1
#define poly_mul_q2 poly_radix_ntt_n1277_q2
#endif

void poly_reduce(poly *a);
void poly_freeze(poly *a);
void poly_fqcsubq(poly *a);

void poly_add(poly *c, const poly *a, const poly *b);
void poly_double(poly *b, const poly *a);
void poly_inverse(poly *b, const poly *a);

void poly_tomont(poly *a);
void poly_frommont(poly *a);

void poly_sample(poly *a, const unsigned char *buf);

void poly_naivemul_q1(poly *c, const poly *a, const poly *b);
void poly_naivemul_q2(poly *c, const poly *a, const poly *b);

void poly_encode_compress(poly *c, const poly *sigma, const unsigned char *m);
void poly_decode(unsigned char *m, const poly *c, const poly *fhat);

#endif
