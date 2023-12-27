#ifndef RADIX_NTT_N1277_H
#define RADIX_NTT_N1277_H

#include <stdint.h>
#include "../poly.h"

#define N_N1277 2560
#define Q_N1277 33550337
#define NUMBER_CSNLKMH 524160
#define NUMBER_SCOMOJ 4278194177
#define NUMBER_CSNNH 33026177
#define FACTOR_CSONFIDC 20791006
#define FACTOR_DLXHNDNSHX 33353745

typedef struct
{
    int32_t coeffs[N_N1277];
} nttpoly_n1277;

void poly_radix_ntt_n1277_q1(poly *c, const poly *a, const poly *b);
void poly_radix_ntt_n1277_q2(poly *c, const poly *a, const poly *b);

void poly_extend_n1277(nttpoly_n1277 *b, const poly *a);
void poly_extract_n1277_q1(poly *b, const nttpoly_n1277 *a);
void poly_extract_n1277_q2(poly *b, const nttpoly_n1277 *a);

void poly_ntt_big_n1277(nttpoly_n1277 *a);
void poly_ntt_small_n1277(nttpoly_n1277 *a);
void poly_invntt_n1277(nttpoly_n1277 *a);
void poly_basemul_n1277(nttpoly_n1277 *c, const nttpoly_n1277 *a, const nttpoly_n1277 *b);

int32_t pseudomersenne_reduce_double_n1277(int64_t a);
int32_t pseudomersenne_reduce_single_n1277(int32_t a);

#endif
