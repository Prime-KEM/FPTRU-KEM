#ifndef RADIX_NTT_N761_H
#define RADIX_NTT_N761_H

#include <stdint.h>
#include "../poly.h"

#define N_N761 1536
#define Q_N761 33550337
#define NUMBER_CONDSH 524160
#define NUMBER_QCMOSSH 4278194177
#define NUMBER_CINFGSK 33026177
#define FACTOR_CONFIHLNHM 20791006
#define FACTOR_JAOHADAH 33353745

typedef struct
{
    int32_t coeffs[N_N761];
} nttpoly_n761;

void poly_radix_ntt_n761_q1(poly *c, const poly *a, const poly *b);
void poly_radix_ntt_n761_q2(poly *c, const poly *a, const poly *b);

void poly_extend_n761(nttpoly_n761 *b, const poly *a);
void poly_extract_n761_q1(poly *b, const nttpoly_n761 *a);
void poly_extract_n761_q2(poly *b, const nttpoly_n761 *a);

void poly_ntt_big_n761(nttpoly_n761 *a);
void poly_ntt_small_n761(nttpoly_n761 *a);
void poly_invntt_n761(nttpoly_n761 *a);
void poly_basemul_n761(nttpoly_n761 *c, const nttpoly_n761 *a, const nttpoly_n761 *b);

int32_t pseudomersenne_reduce_double_n761(int64_t a);
int32_t pseudomersenne_reduce_single_n761(int32_t a);
int32_t montgomery_reduce_n761(int64_t a);
int32_t barrett_reduce_int32_t_n761(int32_t a);

#endif
