#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <math.h>
#include "radix_ntt_n653.h"
#include "../poly.h"
#include "../inverse.h"

int32_t root3_n653[3] = {16128, 2580465, 14180560};

int32_t zetas_n653[N_N653 / 7] = {
    10676304, 16761025, 7394872, 16761025, 11121377, 10949150, 7394872, 16761025, 12927053, 8902443, 8915031, 16113002, 11121377, 10949150, 7394872, 16761025,
    2012145, 7912042, 6709828, 12738274, 1189525, 660953, 16499514, 7058049, 12927053, 8902443, 8915031, 16113002, 11121377, 10949150, 7394872, 16761025,
    11436071, 12085043, 12097024, 13607141, 1189525, 8915031, 16678631, 4692110, 10377718, 13813870, 16499514, 16113002, 10283588, 13149719, 564484, 2963283,
    2012145, 12927053, 16338007, 7725679, 6291901, 3875579, 7912042, 3850100, 15204945, 9193292, 11166066, 16486304, 6709828, 8902443, 15588451, 7583861,
    12085043, 1126178, 8915031, 10949150, 13813870, 12075328, 13149719, 15650975, 12927053, 11121377, 3875579, 14329275, 9193292, 8673499, 8902443, 5655776,
    1126178, 13522707, 12075328, 10649318, 11121377, 7394872, 8673499, 3254446, 13522707, 2596593, 7394872, 16761025, 2596593, 2580465, 2580465, 14180560,
    10676304, 864588, 6965437, 15588906, 11857296, 13045543, 15393923, 12412334, 13795564, 14110054, 1590746, 4257845, 5811151, 3893059, 14859061, 7154973,
    7755271, 600298, 6856840, 1171611, 11091924, 10586117, 9341373, 15532409, 9419128, 12394088, 2974960, 3551173, 2189279, 15415259, 1025148, 4803594,
    3778446, 129973, 6373474, 6243501, 2246922, 13982193, 11735271, 5384557, 14772227, 9387670, 4217375, 10588057, 6370682, 4768624, 14519764, 9751140,
    11436071, 12097024, 660953, 1189525, 16678631, 15489106, 10377718, 16499514, 6121796, 10283588, 564484, 7058049, 2012145, 16338007, 14325862, 6291901,
    7912042, 1620141, 15204945, 11166066, 12738274, 6709828, 15588451, 8878623, 12085043, 8915031, 13607141, 13813870, 13149719, 16113002, 12927053, 3875579,
    7725679, 9193292, 8902443, 16486304, 1126178, 12075328, 10949150, 11121377, 8673499, 14329275, 13522707, 7394872, 10649318, 2596593, 2580465, 16761025};

int32_t zetas_inv_n653[N_N653 / 7] = {
    7026013, 2257389, 12008529, 10406471, 6189096, 12559778, 7389483, 2004926, 11392596, 5041882, 2794960, 14530231, 10533652, 10403679, 16647180, 12998707,
    11973559, 15752005, 1361894, 14587874, 13225980, 13802193, 4383065, 7358025, 1244744, 7435780, 6191036, 5685229, 15605542, 9920313, 16176855, 9021882,
    9622180, 1918092, 12884094, 10966002, 12519308, 15186407, 2667099, 2981589, 4364819, 1383230, 3731610, 4919857, 1188247, 9811716, 15912565, 6100849,
    7898530, 1188702, 10067325, 4038879, 5611087, 1572208, 15157012, 8865111, 10485252, 2451291, 439146, 14765008, 9719104, 16212669, 6493565, 10655357,
    277639, 6399435, 1288047, 98522, 15587628, 16116200, 4680129, 5341082, 290849, 7874710, 7583861, 9051474, 12901574, 3850100, 664151, 3627434,
    2963283, 3170012, 7862122, 4692110, 2447878, 8103654, 5655776, 5828003, 4701825, 15650975, 6127835, 9382281, 3254446, 14196688, 14180560, 16761025,
    7898530, 290849, 1188702, 7583861, 4038879, 7874710, 5611087, 16486304, 15157012, 9051474, 8865111, 3850100, 2451291, 12901574, 439146, 7725679,
    9719104, 664151, 16212669, 2963283, 10655357, 3627434, 277639, 16113002, 1288047, 3170012, 98522, 4692110, 16116200, 7862122, 4680129, 13607141,
    290849, 2447878, 7874710, 5655776, 9051474, 8103654, 12901574, 14329275, 664151, 5828003, 3627434, 15650975, 3170012, 4701825, 7862122, 10949150,
    2447878, 6127835, 8103654, 3254446, 5828003, 9382281, 4701825, 10649318, 6127835, 14196688, 9382281, 16761025, 14196688, 14180560, 14180560, 2580465,
    9719104, 277639, 16116200, 15587628, 4038879, 10067325, 8865111, 14765008, 664151, 7862122, 7874710, 3850100, 5828003, 5655776, 9382281, 16761025,
    664151, 7862122, 7874710, 3850100, 5828003, 5655776, 9382281, 16761025, 5828003, 5655776, 9382281, 16761025, 9382281, 16761025, 16761025};

int32_t pseudomersenne_reduce_double_n653(int64_t a)
{
    int32_t t0, t1;
    int64_t t;

    t0 = a & 0xffffff;
    t1 = a >> 24;
    t = (t1 << 6) + t0 - t1;

    t0 = t & 0xffffff;
    t1 = t >> 24;
    t = (t1 << 6) + t0 - t1 - Q_N653;

    return (int32_t)t;
}

int32_t pseudomersenne_reduce_single_n653(int32_t a)
{
    int32_t t0, t1;

    t0 = a & 0xffffff;
    t1 = a >> 24;
    t0 = (t1 << 6) + t0 - t1 - Q_N653;

    return t0;
}

int32_t montgomery_reduce_n653(int64_t a)
{
    int32_t t;
    t = (int32_t)a * NUMBER_ACONFI;
    t = (a - (int64_t)t * Q_N653) >> 32;
    return t;
}

void ntt_big_n653(int32_t a[N_N653])
{
    unsigned int len, start, j, k;
    int32_t zeta, t;
    int32_t tb, tc, tpho, zeta1, zeta2;

    len = N_N653 / 2;
    for (start = 0; start < N_N653; start = j + len)
    {
        for (j = start; j < start + len; ++j)
        {
            t = a[j + len];
            a[j + len] = (a[j] + t);
            a[j] = (a[j] - t);
        }
    }

    k = 2;

    for (len = N_N653 / 4; len >= 42; len >>= 1)
    {
        for (start = 0; start < N_N653; start = j + len)
        {
            zeta = zetas_n653[k++];

            if (zeta == NUMBER_AKNCF)
            {
                for (j = start; j < start + len; ++j)
                {
                    t = a[j + len];
                    a[j + len] = (a[j] + t);
                    a[j] = (a[j] - t);
                }
            }
            else
            {
                for (j = start; j < start + len; ++j)
                {
                    t = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
                    a[j + len] = (a[j] - t);
                    a[j] = (a[j] + t);
                }
            }
        }
    }

    len = 14;
    for (start = 0; start < N_N653; start += 3 * len)
    {
        zeta1 = zetas_n653[k++];
        zeta2 = zetas_n653[k++];
        for (j = start; j < start + len; ++j)
        {
            tb = montgomery_reduce_n653((int64_t)zeta1 * a[j + len]);
            tc = montgomery_reduce_n653((int64_t)zeta2 * a[j + len + len]);
            tpho = montgomery_reduce_n653((int64_t)root3_n653[1] * (tb - tc));

            a[j + len + len] = (a[j] - tb - tpho);
            a[j + len] = (a[j] - tc + tpho);
            a[j] = (a[j] + tb + tc);
        }
    }

    len = 7;
    for (start = 0; start < N_N653; start = j + len)
    {
        zeta = zetas_n653[k++];

        if (zeta == NUMBER_AKNCF)
        {
            for (j = start; j < start + len; ++j)
            {
                t = a[j + len];
                a[j + len] = (a[j] + t);
                a[j] = (a[j] - t);
            }
        }
        else
        {
            for (j = start; j < start + len; ++j)
            {
                t = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
                a[j + len] = pseudomersenne_reduce_single_n653(a[j] - t);
                a[j] = pseudomersenne_reduce_single_n653(a[j] + t);
            }
        }
    }
}

void ntt_small_n653(int32_t a[N_N653])
{
    unsigned int len, start, j, k;
    int32_t zeta, t;
    int32_t tb, tc, tpho, zeta1, zeta2;

    len = N_N653 / 2;
    for (start = 0; start < N_N653; start = j + len)
    {
        for (j = start; j < start + len; ++j)
        {
            t = a[j + len];
            a[j + len] = (a[j] + t);
            a[j] = (a[j] - t);
        }
    }

    k = 2;

    len = N_N653 / 4;
    start = 0;
    for (j = start; j < start + len; ++j)
    {
        t = FACTOR_CNSHHUI * a[j + len];
        a[j + len] = (a[j] - t);
        a[j] = (a[j] + t);
    }

    start += 2 * len;
    for (j = start; j < start + len; ++j)
    {
        t = a[j + len];
        a[j + len] = (a[j] + t);
        a[j] = (a[j] - t);
    }
    k += 2;

    for (len = N_N653 / 8; len >= 42; len >>= 1)
    {
        for (start = 0; start < N_N653; start = j + len)
        {
            zeta = zetas_n653[k++];

            if (zeta == NUMBER_AKNCF)
            {
                for (j = start; j < start + len; ++j)
                {
                    t = a[j + len];
                    a[j + len] = (a[j] + t);
                    a[j] = (a[j] - t);
                }
            }
            else
            {
                for (j = start; j < start + len; ++j)
                {
                    t = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
                    a[j + len] = (a[j] - t);
                    a[j] = (a[j] + t);
                }
            }
        }
    }

    len = 14;
    for (start = 0; start < N_N653; start += 3 * len)
    {
        zeta1 = zetas_n653[k++];
        zeta2 = zetas_n653[k++];
        for (j = start; j < start + len; ++j)
        {
            tb = montgomery_reduce_n653((int64_t)zeta1 * a[j + len]);
            tc = montgomery_reduce_n653((int64_t)zeta2 * a[j + len + len]);
            tpho = montgomery_reduce_n653((int64_t)root3_n653[1] * (tb - tc));

            a[j + len + len] = (a[j] - tb - tpho);
            a[j + len] = (a[j] - tc + tpho);
            a[j] = (a[j] + tb + tc);
        }
    }

    len = 7;
    for (start = 0; start < N_N653; start = j + len)
    {
        zeta = zetas_n653[k++];

        if (zeta == NUMBER_AKNCF)
        {
            for (j = start; j < start + len; ++j)
            {
                t = a[j + len];
                a[j + len] = (a[j] + t);
                a[j] = (a[j] - t);
            }
        }
        else
        {
            for (j = start; j < start + len; ++j)
            {
                t = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
                a[j + len] = pseudomersenne_reduce_single_n653(a[j] - t);
                a[j] = pseudomersenne_reduce_single_n653(a[j] + t);
            }
        }
    }
}

void invntt_tomont_n653(int32_t a[N_N653])
{
    unsigned int start, len, j, k;
    int32_t t, zeta;
    int32_t tb, tc, tpho, zeta1, zeta2;

    k = 0;

    len = 7;
    for (start = 0; start < N_N653; start = j + len)
    {
        zeta = zetas_inv_n653[k++];

        if (zeta == NUMBER_AKNCF)
        {
            for (j = start; j < start + len; ++j)
            {
                t = a[j];
                a[j] = (a[j + len] + t);
                a[j + len] = (a[j + len] - t);
            }
        }
        else
        {
            for (j = start; j < start + len; ++j)
            {
                t = a[j];
                a[j] = (t + a[j + len]);
                a[j + len] = (t - a[j + len]);
                a[j + len] = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
            }
        }
    }

    len = 14;
    for (start = 0; start < N_N653; start += 3 * len)
    {
        zeta1 = zetas_inv_n653[k++];
        zeta2 = zetas_inv_n653[k++];
        for (j = start; j < start + len; ++j)
        {
            tpho = montgomery_reduce_n653((int64_t)root3_n653[1] * (a[j + len] - a[j + len + len]));
            tb = (a[j] - a[j + len] - tpho);
            tc = (a[j] - a[j + len + len] + tpho);

            a[j] = (a[j] + a[j + len] + a[j + len + len]);
            a[j + len] = montgomery_reduce_n653((int64_t)zeta1 * tb);
            a[j + len + len] = montgomery_reduce_n653((int64_t)zeta2 * tc);
        }
    }

    for (len = 42; len <= N_N653 / 4; len <<= 1)
    {
        for (start = 0; start < N_N653; start = j + len)
        {
            zeta = zetas_inv_n653[k++];

            if (zeta == NUMBER_AKNCF)
            {
                for (j = start; j < start + len; ++j)
                {
                    t = a[j];
                    a[j] = (a[j + len] + t);
                    a[j + len] = (a[j + len] - t);
                }
            }
            else
            {
                for (j = start; j < start + len; ++j)
                {
                    t = a[j];
                    a[j] = (t + a[j + len]);
                    a[j + len] = (t - a[j + len]);
                    a[j + len] = montgomery_reduce_n653((int64_t)zeta * a[j + len]);
                }
            }
        }
    }

    len = N_N653 / 2;
    for (start = 0; start < N_N653; start = j + len)
    {
        zeta = zetas_inv_n653[k++];
        for (j = start; j < start + len; ++j)
        {
            t = a[j];
            a[j] = pseudomersenne_reduce_single_n653(a[j + len] + t);
            a[j + len] = pseudomersenne_reduce_single_n653(a[j + len] - t);
        }
    }
}

#define KARA(a, b, x, y, d) ((montgomery_reduce_n653((int64_t)(a[x] + a[y]) * (b[x] + b[y])) - d[x] - d[y]))

void basemul_n653(int32_t c[7], const int32_t a[7], const int32_t b[7], const int32_t zeta)
{
    int i;
    int32_t d[7];

    for (i = 0; i < 7; i++)
        d[i] = montgomery_reduce_n653((int64_t)a[i] * b[i]);

    c[0] = (d[0] + montgomery_reduce_n653((int64_t)zeta * (KARA(a, b, 1, 6, d) + KARA(a, b, 2, 5, d) + KARA(a, b, 3, 4, d))));
    c[1] = (KARA(a, b, 0, 1, d) + montgomery_reduce_n653((int64_t)zeta * (KARA(a, b, 2, 6, d) + KARA(a, b, 3, 5, d) + d[4])));
    c[2] = (KARA(a, b, 0, 2, d) + d[1] + montgomery_reduce_n653((int64_t)zeta * (KARA(a, b, 3, 6, d) + KARA(a, b, 4, 5, d))));
    c[3] = (KARA(a, b, 0, 3, d) + KARA(a, b, 1, 2, d) + montgomery_reduce_n653((int64_t)zeta * (KARA(a, b, 4, 6, d) + d[5])));
    c[4] = (KARA(a, b, 0, 4, d) + KARA(a, b, 1, 3, d) + d[2] + montgomery_reduce_n653((int64_t)zeta * (KARA(a, b, 5, 6, d))));
    c[5] = (KARA(a, b, 0, 5, d) + KARA(a, b, 1, 4, d) + KARA(a, b, 2, 3, d) + montgomery_reduce_n653((int64_t)zeta * d[6]));
    c[6] = (KARA(a, b, 0, 6, d) + KARA(a, b, 1, 5, d) + KARA(a, b, 2, 4, d) + d[3]);
}

void poly_ntt_big_n653(nttpoly_n653 *a)
{
    ntt_big_n653(a->coeffs);
}

void poly_ntt_small_n653(nttpoly_n653 *a)
{
    ntt_small_n653(a->coeffs);
}

void poly_invntt_n653(nttpoly_n653 *a)
{
    invntt_tomont_n653(a->coeffs);
}

void poly_basemul_n653(nttpoly_n653 *c, const nttpoly_n653 *a, const nttpoly_n653 *b)
{
    int i;
    int32_t zeta;
    for (i = 0; i < N_N653 / 14; i++)
    {
        zeta = zetas_n653[N_N653 / 14 + i];
        basemul_n653(c->coeffs + 14 * i, a->coeffs + 14 * i, b->coeffs + 14 * i, zeta);
        basemul_n653(c->coeffs + 14 * i + 7, a->coeffs + 14 * i + 7, b->coeffs + 14 * i + 7, -zeta);
    }
}

void poly_extend_n653(nttpoly_n653 *b, const poly *a)
{
    int i;
    for (i = 0; i < FPTRU_N; i++)
        b->coeffs[i] = a->coeffs[i];
    for (; i < N_N653; i++)
        b->coeffs[i] = 0;
}

void poly_extract_n653_q1(poly *b, const nttpoly_n653 *a)
{
    b->coeffs[0] = fq_freeze(montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[0] + a->coeffs[FPTRU_N])));
    for (int i = 1; i <= FPTRU_N - 2; i++)
        b->coeffs[i] = fq_freeze(montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[i] + a->coeffs[i + FPTRU_N - 1] + a->coeffs[i + FPTRU_N])));
    b->coeffs[FPTRU_N - 1] = fq_freeze(montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[FPTRU_N - 1] + a->coeffs[2 * FPTRU_N - 2])));
}

void poly_extract_n653_q2(poly *b, const nttpoly_n653 *a)
{
    b->coeffs[0] = (montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[0] + a->coeffs[FPTRU_N]))) & (FPTRU_Q2 - 1);
    for (int i = 1; i <= FPTRU_N - 2; i++)
        b->coeffs[i] = (montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[i] + a->coeffs[i + FPTRU_N - 1] + a->coeffs[i + FPTRU_N]))) & (FPTRU_Q2 - 1);
    b->coeffs[FPTRU_N - 1] = (montgomery_reduce_n653((int64_t)FACTOR_NAKYFHAIRNF * (a->coeffs[FPTRU_N - 1] + a->coeffs[2 * FPTRU_N - 2]))) & (FPTRU_Q2 - 1);
}

void poly_radix_ntt_n653_q1(poly *c, const poly *a, const poly *b)
{
    nttpoly_n653 ntta, nttb, nttc;

    poly_extend_n653(&ntta, a);
    poly_extend_n653(&nttb, b);

    poly_ntt_big_n653(&ntta);
    poly_ntt_small_n653(&nttb);
    poly_basemul_n653(&nttc, &ntta, &nttb);
    poly_invntt_n653(&nttc);

    poly_extract_n653_q1(c, &nttc);
}

void poly_radix_ntt_n653_q2(poly *c, const poly *a, const poly *b)
{
    nttpoly_n653 ntta, nttb, nttc;

    poly_extend_n653(&ntta, a);
    poly_extend_n653(&nttb, b);

    poly_ntt_big_n653(&ntta);
    poly_ntt_small_n653(&nttb);
    poly_basemul_n653(&nttc, &ntta, &nttb);
    poly_invntt_n653(&nttc);

    poly_extract_n653_q2(c, &nttc);
}