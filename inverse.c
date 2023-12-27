#include <stdint.h>
#include "params.h"
#include "reduce.h"
#include "poly.h"

/* Based on the reference implementation of NTRU Prime (NIST 3rd round submission)
 * by Daniel J. Bernstein, Chitchanok Chuengsatiansup, Tanja Lange, Christine van Vredendaal.
 * It can be used for q \in {4091, 4621, 4591, 7879}.
 * */

void uint32_divmod_uint14(uint32_t *y, uint16_t *r, uint32_t x, uint16_t m)
{
    uint32_t w = 0x80000000;
    uint32_t qpart;
    uint32_t mask;

    w /= m;

    *y = 0;
    qpart = (x * (uint64_t)w) >> 31;
    x -= qpart * m;
    *y += qpart;

    qpart = (x * (uint64_t)w) >> 31;
    x -= qpart * m;
    *y += qpart;

    x -= m;
    *y += 1;
    mask = -(x >> 31);
    x += mask & (uint32_t)m;
    *y += mask;

    *r = x;
}

void int32_divmod_uint14(int32_t *y, uint16_t *r, int32_t x, uint16_t m)
{
    uint32_t uq, uq2;
    uint16_t ur, ur2;
    uint32_t mask;

    uint32_divmod_uint14(&uq, &ur, 0x80000000 + (uint32_t)x, m);
    uint32_divmod_uint14(&uq2, &ur2, 0x80000000, m);

    ur -= ur2;
    uq -= uq2;

    mask = -(uint32_t)(ur >> 15);
    ur += mask & m;
    uq += mask;
    *r = ur;
    *y = uq;
}

uint16_t uint32_mod_uint14(uint32_t x, uint16_t m)
{
    uint32_t q;
    uint16_t r;
    uint32_divmod_uint14(&q, &r, x, m);
    return r;
}

uint16_t int32_mod_uint14(int32_t x, uint16_t m)
{
    int32_t y;
    uint16_t r;
    int32_divmod_uint14(&y, &r, x, m);
    return r;
}

int16_t fq_freeze(int32_t x)
{

    const int16_t half_q = (FPTRU_Q - 1) >> 1;
    return int32_mod_uint14(x + half_q, FPTRU_Q) - half_q;
}

int int16_nonzero_mask(int16_t x)
{
    uint16_t u = x;
    uint32_t w = u;
    w = -w;
    w >>= 31;
    return -w;
}

int int16_negative_mask(int16_t x)
{
    uint16_t u = x;
    u >>= 15;
    return -(int)u;
}

#if (FPTRU_Q == 4091)

int16_t fq_inverse(int16_t a0)
{
    int16_t a = a0;
    int16_t t = a0;

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    return t;
}

#elif (FPTRU_Q == 4591)

int16_t fq_inverse(int16_t a0)
{
    int16_t a = a0;
    int16_t t = a0;

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    return t;
}

#elif (FPTRU_Q == 4621)

int16_t fq_inverse(int16_t a0)
{
    int16_t a = a0;
    int16_t t = a0;

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    return t;
}
#elif (FPTRU_Q == 7879)

int16_t fq_inverse(int16_t a0)
{
    int16_t a = a0;
    int16_t t = a0;

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    a = fq_freeze(a * (int32_t)a);
    t = fq_freeze(t * (int32_t)a);

    return t;
}
#endif

int rq_inverse(int16_t *finv, const int16_t *f)
{
    int16_t Phi[FPTRU_N + 1], F[FPTRU_N + 1], V[FPTRU_N + 1], S[FPTRU_N + 1];
    int i, loop, Delta, swap, t;
    int32_t Phi0, F0;
    int16_t scale;

    for (i = 0; i < FPTRU_N; ++i)
        Phi[i] = 0;
    Phi[0] = 1;
    Phi[FPTRU_N - 1] = Phi[FPTRU_N] = -1;

    for (i = 0; i < FPTRU_N; ++i)
        F[FPTRU_N - 1 - i] = f[i];
    F[FPTRU_N] = 0;

    Delta = 1;

    for (i = 0; i < FPTRU_N + 1; ++i)
        V[i] = 0;

    for (i = 0; i < FPTRU_N + 1; ++i)
        S[i] = 0;
    S[0] = 1;

    for (loop = 0; loop < 2 * FPTRU_N - 1; ++loop)
    {
        for (i = FPTRU_N; i > 0; --i)
            V[i] = V[i - 1];
        V[0] = 0;

        swap = int16_negative_mask(-Delta) & int16_nonzero_mask(F[0]);

        for (i = 0; i < FPTRU_N + 1; ++i)
        {
            t = swap & (Phi[i] ^ F[i]);
            Phi[i] ^= t;
            F[i] ^= t;
            t = swap & (V[i] ^ S[i]);
            V[i] ^= t;
            S[i] ^= t;
        }

        Delta ^= swap & (Delta ^ -Delta);
        Delta++;

        Phi0 = Phi[0];
        F0 = F[0];

        for (i = 0; i < FPTRU_N + 1; ++i)
            F[i] = fq_freeze(Phi0 * F[i] - F0 * Phi[i]);
        for (i = 0; i < FPTRU_N; ++i)
            F[i] = F[i + 1];
        F[FPTRU_N] = 0;

        for (i = 0; i < FPTRU_N + 1; ++i)
            S[i] = fq_freeze(Phi0 * S[i] - F0 * V[i]);
    }

    scale = fq_inverse(Phi[0]);
    for (i = 0; i < FPTRU_N; ++i)
        finv[i] = fq_freeze(scale * (int32_t)V[FPTRU_N - 1 - i]);

    return int16_nonzero_mask(Delta);
}
