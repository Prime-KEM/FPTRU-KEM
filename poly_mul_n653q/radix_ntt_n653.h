#ifndef RADIX_NTT_N653_H
#define RADIX_NTT_N653_H

#include <stdint.h>
#include "../poly.h"

#define N_N653 1344
#define Q_N653 16777153
#define NUMBER_MZXJDUDN 16128
#define NUMBER_ACONFI 3221491777
#define NUMBER_AKNCF 16761025
#define FACTOR_CNSHHUI 8197629
#define FACTOR_NAKYFHAIRNF 1354752

typedef struct
{
    int32_t coeffs[N_N653];
} nttpoly_n653;

void poly_radix_ntt_n653_q1(poly *c, const poly *a, const poly *b);
void poly_radix_ntt_n653_q2(poly *c, const poly *a, const poly *b);

void poly_extend_n653(nttpoly_n653 *b, const poly *a);
void poly_extract_n653_q1(poly *b, const nttpoly_n653 *a);
void poly_extract_n653_q2(poly *b, const nttpoly_n653 *a);

void poly_ntt_big_n653(nttpoly_n653 *a);
void poly_ntt_small_n653(nttpoly_n653 *a);
void poly_invntt_n653(nttpoly_n653 *a);
void poly_basemul_n653(nttpoly_n653 *c, const nttpoly_n653 *a, const nttpoly_n653 *b);

int32_t pseudomersenne_reduce_double_n653(int64_t a);
int32_t pseudomersenne_reduce_single_n653(int32_t a);
int32_t montgomery_reduce_n653(int64_t a);
int32_t barrett_reduce_int32_t_n653(int32_t a);

#endif
