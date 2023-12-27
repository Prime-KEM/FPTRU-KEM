#include "pack.h"
#include "inverse.h"

#if (USING_PK_ENCODE == 1)
/* Based on the reference implementation of NTRU Prime (NIST 3rd round submission)
 * by Daniel J. Bernstein, Chitchanok Chuengsatiansup, Tanja Lange, Christine van Vredendaal.
 * It can be used for q \in {4091, 4621, 4591, 7879}.
 * */

void Encode(unsigned char *out, const uint16_t *R, const uint16_t *M, const long long len)
{
    if (len == 1)
    {
        uint16_t r = R[0];
        uint16_t m = M[0];
        while (m > 1)
        {
            *out++ = r;
            r >>= 8;
            m = (m + 255) >> 8;
        }
    }

    if (len > 1)
    {
        uint16_t R2[(len + 1) / 2];
        uint16_t M2[(len + 1) / 2];
        long long i;

        for (i = 0; i < len - 1; i += 2)
        {
            uint32_t m0 = M[i];
            uint32_t r = R[i] + R[i + 1] * m0;
            uint32_t m = M[i + 1] * m0;

            while (m >= 16384)
            {
                *out++ = r;
                r >>= 8;
                m = (m + 255) >> 8;
            }
            R2[i / 2] = r;
            M2[i / 2] = m;
        }

        if (i < len)
        {
            R2[i / 2] = R[i];
            M2[i / 2] = M[i];
        }

        Encode(out, R2, M2, (len + 1) / 2);
    }
}

void pack_pk(unsigned char *r, const poly *a)
{
    uint16_t R[FPTRU_N], M[FPTRU_N];

    for (int i = 0; i < FPTRU_N; ++i)
        R[i] = (uint16_t)a->coeffs[i];

    for (int i = 0; i < FPTRU_N; ++i)
        M[i] = FPTRU_Q;

    Encode(r, R, M, FPTRU_N);
}
#endif

#if (USING_PK_ENCODE == 1)
void Decode(uint16_t *out, const unsigned char *S, const uint16_t *M, const long long len)
{
    if (len == 1)
    {
        if (M[0] == 1)
            *out = 0;
        else if (M[0] <= 256)
            *out = uint32_mod_uint14(S[0], M[0]);
        else
            *out = uint32_mod_uint14(S[0] + (((uint16_t)S[1]) << 8), M[0]);
    }

    if (len > 1)
    {
        uint16_t R2[(len + 1) / 2];
        uint16_t M2[(len + 1) / 2];
        uint16_t bottomr[len / 2];
        uint32_t bottomt[len / 2];
        long long i;
        for (i = 0; i < len - 1; i += 2)
        {
            uint32_t m = M[i] * (uint32_t)M[i + 1];
            if (m > 256 * 16383)
            {
                bottomt[i / 2] = 256 * 256;
                bottomr[i / 2] = S[0] + 256 * S[1];
                S += 2;
                M2[i / 2] = (((m + 255) >> 8) + 255) >> 8;
            }
            else if (m >= 16384)
            {
                bottomt[i / 2] = 256;
                bottomr[i / 2] = S[0];
                S += 1;
                M2[i / 2] = (m + 255) >> 8;
            }
            else
            {
                bottomt[i / 2] = 1;
                bottomr[i / 2] = 0;
                M2[i / 2] = m;
            }
        }
        if (i < len)
            M2[i / 2] = M[i];

        Decode(R2, S, M2, (len + 1) / 2);

        for (i = 0; i < len - 1; i += 2)
        {
            uint32_t r = bottomr[i / 2];
            uint32_t r1;
            uint16_t r0;
            r += bottomt[i / 2] * R2[i / 2];
            uint32_divmod_uint14(&r1, &r0, r, M[i]);
            r1 = uint32_mod_uint14(r1, M[i + 1]);
            *out++ = r0;
            *out++ = r1;
        }
        if (i < len)
            *out++ = R2[i / 2];
    }
}

void unpack_pk(poly *r, const unsigned char *a)
{
    uint16_t R[FPTRU_N], M[FPTRU_N];

    for (int i = 0; i < FPTRU_N; ++i)
        M[i] = FPTRU_Q;

    Decode(R, a, M, FPTRU_N);

    for (int i = 0; i < FPTRU_N; ++i)
        r->coeffs[i] = ((int16_t)R[i]);
}
#endif

void pack_sk(unsigned char *r, const poly *a)
{
    unsigned int i;
    uint8_t t[8];
#if ((FPTRU_N == 653) || (FPTRU_N == 1277))
    for (i = 0; i < FPTRU_N / 8; i++)
    {
        t[0] = FPTRU_BOUND - a->coeffs[8 * i + 0];
        t[1] = FPTRU_BOUND - a->coeffs[8 * i + 1];
        t[2] = FPTRU_BOUND - a->coeffs[8 * i + 2];
        t[3] = FPTRU_BOUND - a->coeffs[8 * i + 3];
        t[4] = FPTRU_BOUND - a->coeffs[8 * i + 4];
        t[5] = FPTRU_BOUND - a->coeffs[8 * i + 5];
        t[6] = FPTRU_BOUND - a->coeffs[8 * i + 6];
        t[7] = FPTRU_BOUND - a->coeffs[8 * i + 7];

        r[4 * i + 0] = (t[0] >> 0) | (t[1] << 4);
        r[4 * i + 1] = (t[2] >> 0) | (t[3] << 4);
        r[4 * i + 2] = (t[4] >> 0) | (t[5] << 4);
        r[4 * i + 3] = (t[6] << 0) | (t[7] << 4);
    }
    t[0] = FPTRU_BOUND - a->coeffs[8 * i + 0];
    t[1] = FPTRU_BOUND - a->coeffs[8 * i + 1];
    t[2] = FPTRU_BOUND - a->coeffs[8 * i + 2];
    t[3] = FPTRU_BOUND - a->coeffs[8 * i + 3];
    t[4] = FPTRU_BOUND - a->coeffs[8 * i + 4];
    r[4 * i + 0] = (t[0] >> 0) | (t[1] << 4);
    r[4 * i + 1] = (t[2] >> 0) | (t[3] << 4);
    r[4 * i + 2] = (t[4] >> 0) & 0xf;
#elif (FPTRU_N == 761)
    for (i = 0; i < FPTRU_N / 8; i++)
    {
        t[0] = FPTRU_BOUND - a->coeffs[8 * i + 0];
        t[1] = FPTRU_BOUND - a->coeffs[8 * i + 1];
        t[2] = FPTRU_BOUND - a->coeffs[8 * i + 2];
        t[3] = FPTRU_BOUND - a->coeffs[8 * i + 3];
        t[4] = FPTRU_BOUND - a->coeffs[8 * i + 4];
        t[5] = FPTRU_BOUND - a->coeffs[8 * i + 5];
        t[6] = FPTRU_BOUND - a->coeffs[8 * i + 6];
        t[7] = FPTRU_BOUND - a->coeffs[8 * i + 7];

        r[4 * i + 0] = (t[0] >> 0) | (t[1] << 4);
        r[4 * i + 1] = (t[2] >> 0) | (t[3] << 4);
        r[4 * i + 2] = (t[4] >> 0) | (t[5] << 4);
        r[4 * i + 3] = (t[6] << 0) | (t[7] << 4);
    }
    t[0] = FPTRU_BOUND - a->coeffs[8 * i + 0];
    r[4 * i + 0] = (t[0] >> 0) & 0xf;
#endif
}

void unpack_sk(poly *r, const unsigned char *a)
{
    unsigned int i;
#if ((FPTRU_N == 653) || (FPTRU_N == 1277))
    for (i = 0; i < FPTRU_N / 8; ++i)
    {
        r->coeffs[8 * i + 0] = (a[4 * i + 0] >> 0) & 15;
        r->coeffs[8 * i + 1] = (a[4 * i + 0] >> 4) & 15;
        r->coeffs[8 * i + 2] = (a[4 * i + 1] >> 0) & 15;
        r->coeffs[8 * i + 3] = (a[4 * i + 1] >> 4) & 15;
        r->coeffs[8 * i + 4] = (a[4 * i + 2] >> 0) & 15;
        r->coeffs[8 * i + 5] = (a[4 * i + 2] >> 4) & 15;
        r->coeffs[8 * i + 6] = (a[4 * i + 3] >> 0) & 15;
        r->coeffs[8 * i + 7] = (a[4 * i + 3] >> 4) & 15;

        r->coeffs[8 * i + 0] = FPTRU_BOUND - r->coeffs[8 * i + 0];
        r->coeffs[8 * i + 1] = FPTRU_BOUND - r->coeffs[8 * i + 1];
        r->coeffs[8 * i + 2] = FPTRU_BOUND - r->coeffs[8 * i + 2];
        r->coeffs[8 * i + 3] = FPTRU_BOUND - r->coeffs[8 * i + 3];
        r->coeffs[8 * i + 4] = FPTRU_BOUND - r->coeffs[8 * i + 4];
        r->coeffs[8 * i + 5] = FPTRU_BOUND - r->coeffs[8 * i + 5];
        r->coeffs[8 * i + 6] = FPTRU_BOUND - r->coeffs[8 * i + 6];
        r->coeffs[8 * i + 7] = FPTRU_BOUND - r->coeffs[8 * i + 7];
    }
    r->coeffs[8 * i + 0] = (a[4 * i + 0] >> 0) & 15;
    r->coeffs[8 * i + 1] = (a[4 * i + 0] >> 4) & 15;
    r->coeffs[8 * i + 2] = (a[4 * i + 1] >> 0) & 15;
    r->coeffs[8 * i + 3] = (a[4 * i + 1] >> 4) & 15;
    r->coeffs[8 * i + 4] = (a[4 * i + 2] >> 0) & 15;
    r->coeffs[8 * i + 0] = FPTRU_BOUND - r->coeffs[8 * i + 0];
    r->coeffs[8 * i + 1] = FPTRU_BOUND - r->coeffs[8 * i + 1];
    r->coeffs[8 * i + 2] = FPTRU_BOUND - r->coeffs[8 * i + 2];
    r->coeffs[8 * i + 3] = FPTRU_BOUND - r->coeffs[8 * i + 3];
    r->coeffs[8 * i + 4] = FPTRU_BOUND - r->coeffs[8 * i + 4];
#elif (FPTRU_N == 761)
    for (i = 0; i < FPTRU_N / 8; ++i)
    {
        r->coeffs[8 * i + 0] = (a[4 * i + 0] >> 0) & 15;
        r->coeffs[8 * i + 1] = (a[4 * i + 0] >> 4) & 15;
        r->coeffs[8 * i + 2] = (a[4 * i + 1] >> 0) & 15;
        r->coeffs[8 * i + 3] = (a[4 * i + 1] >> 4) & 15;
        r->coeffs[8 * i + 4] = (a[4 * i + 2] >> 0) & 15;
        r->coeffs[8 * i + 5] = (a[4 * i + 2] >> 4) & 15;
        r->coeffs[8 * i + 6] = (a[4 * i + 3] >> 0) & 15;
        r->coeffs[8 * i + 7] = (a[4 * i + 3] >> 4) & 15;

        r->coeffs[8 * i + 0] = FPTRU_BOUND - r->coeffs[8 * i + 0];
        r->coeffs[8 * i + 1] = FPTRU_BOUND - r->coeffs[8 * i + 1];
        r->coeffs[8 * i + 2] = FPTRU_BOUND - r->coeffs[8 * i + 2];
        r->coeffs[8 * i + 3] = FPTRU_BOUND - r->coeffs[8 * i + 3];
        r->coeffs[8 * i + 4] = FPTRU_BOUND - r->coeffs[8 * i + 4];
        r->coeffs[8 * i + 5] = FPTRU_BOUND - r->coeffs[8 * i + 5];
        r->coeffs[8 * i + 6] = FPTRU_BOUND - r->coeffs[8 * i + 6];
        r->coeffs[8 * i + 7] = FPTRU_BOUND - r->coeffs[8 * i + 7];
    }
    r->coeffs[8 * i + 0] = (a[4 * i + 0] >> 0) & 15;
    r->coeffs[8 * i + 0] = FPTRU_BOUND - r->coeffs[8 * i + 0];
#endif
}

void pack_ct(unsigned char *r, const poly *a)
{
    unsigned int i;
#if (FPTRU_Q2 == 1024)
    for (i = 0; i < FPTRU_N / 4; ++i)
    {
        r[5 * i + 0] = (a->coeffs[4 * i + 0] >> 0);
        r[5 * i + 1] = (a->coeffs[4 * i + 0] >> 8) | ((int16_t)a->coeffs[4 * i + 1] << 2);
        r[5 * i + 2] = (a->coeffs[4 * i + 1] >> 6) | ((int16_t)a->coeffs[4 * i + 2] << 4);
        r[5 * i + 3] = (a->coeffs[4 * i + 2] >> 4) | ((int16_t)a->coeffs[4 * i + 3] << 6);
        r[5 * i + 4] = (a->coeffs[4 * i + 3] >> 2);
    }
    r[5 * i + 0] = (a->coeffs[4 * i + 0] >> 0);
    r[5 * i + 1] = (a->coeffs[4 * i + 0] >> 8) & 0x3;
#elif (FPTRU_Q2 == 2048)
    for (i = 0; i < FPTRU_N / 8; ++i)
    {
        r[11 * i + 0] = a->coeffs[8 * i + 0] >> 0;
        r[11 * i + 1] = a->coeffs[8 * i + 0] >> 8 | ((int16_t)a->coeffs[8 * i + 1] << 3);
        r[11 * i + 2] = a->coeffs[8 * i + 1] >> 5 | ((int16_t)a->coeffs[8 * i + 2] << 6);
        r[11 * i + 3] = a->coeffs[8 * i + 2] >> 2;
        r[11 * i + 4] = a->coeffs[8 * i + 2] >> 10 | ((int16_t)a->coeffs[8 * i + 3] << 1);
        r[11 * i + 5] = a->coeffs[8 * i + 3] >> 7 | ((int16_t)a->coeffs[8 * i + 4] << 4);
        r[11 * i + 6] = a->coeffs[8 * i + 4] >> 4 | ((int16_t)a->coeffs[8 * i + 5] << 7);
        r[11 * i + 7] = a->coeffs[8 * i + 5] >> 1;
        r[11 * i + 8] = a->coeffs[8 * i + 5] >> 9 | ((int16_t)a->coeffs[8 * i + 6] << 2);
        r[11 * i + 9] = a->coeffs[8 * i + 6] >> 6 | ((int16_t)a->coeffs[8 * i + 7] << 5);
        r[11 * i + 10] = a->coeffs[8 * i + 7] >> 3;
    }
    r[11 * i + 0] = a->coeffs[8 * i + 0] >> 0;
    r[11 * i + 1] = a->coeffs[8 * i + 0] >> 8 | ((int16_t)a->coeffs[8 * i + 1] << 3);
    r[11 * i + 2] = a->coeffs[8 * i + 1] >> 5 | ((int16_t)a->coeffs[8 * i + 2] << 6);
    r[11 * i + 3] = a->coeffs[8 * i + 2] >> 2;
    r[11 * i + 4] = a->coeffs[8 * i + 2] >> 10 | ((int16_t)a->coeffs[8 * i + 3] << 1);
    r[11 * i + 5] = a->coeffs[8 * i + 3] >> 7 | ((int16_t)a->coeffs[8 * i + 4] << 4);
    r[11 * i + 6] = (a->coeffs[8 * i + 4] >> 4) & 0x7f;
#elif (FPTRU_Q2 == 4096)
    for (i = 0; i < FPTRU_N / 2; i++)
    {
        r[3 * i + 0] = (a->coeffs[2 * i] >> 0);
        r[3 * i + 1] = (a->coeffs[2 * i] >> 8) | (a->coeffs[2 * i + 1] << 4);
        r[3 * i + 2] = (a->coeffs[2 * i + 1] >> 4);
    }
    r[3 * i + 0] = (a->coeffs[2 * i] >> 0);
    r[3 * i + 1] = (a->coeffs[2 * i] >> 8) & 0xf;
#endif
}

void unpack_ct(poly *r, const unsigned char *a)
{
    unsigned int i;
#if (FPTRU_Q2 == 1024)
    for (i = 0; i < FPTRU_N / 4; ++i)
    {
        r->coeffs[4 * i + 0] = ((a[5 * i + 0] >> 0) | ((uint16_t)a[5 * i + 1] << 8)) & 0x3FF;
        r->coeffs[4 * i + 1] = ((a[5 * i + 1] >> 2) | ((uint16_t)a[5 * i + 2] << 6)) & 0x3FF;
        r->coeffs[4 * i + 2] = ((a[5 * i + 2] >> 4) | ((uint16_t)a[5 * i + 3] << 4)) & 0x3FF;
        r->coeffs[4 * i + 3] = ((a[5 * i + 3] >> 6) | ((uint16_t)a[5 * i + 4] << 2)) & 0x3FF;
    }
    r->coeffs[4 * i + 0] = ((a[5 * i + 0] >> 0) | ((uint16_t)a[5 * i + 1] << 8)) & 0x3FF;
#elif (FPTRU_Q2 == 2048)
    for (i = 0; i < FPTRU_N / 8; ++i)
    {
        r->coeffs[8 * i + 0] = ((a[11 * i + 0] >> 0) | ((uint16_t)a[11 * i + 1] << 8)) & 0x7FF;
        r->coeffs[8 * i + 1] = ((a[11 * i + 1] >> 3) | ((uint16_t)a[11 * i + 2] << 5)) & 0x7FF;
        r->coeffs[8 * i + 2] = ((a[11 * i + 2] >> 6) | ((uint16_t)a[11 * i + 3] << 2) | ((uint16_t)a[11 * i + 4] << 10)) & 0x7FF;
        r->coeffs[8 * i + 3] = ((a[11 * i + 4] >> 1) | ((uint16_t)a[11 * i + 5] << 7)) & 0x7FF;
        r->coeffs[8 * i + 4] = ((a[11 * i + 5] >> 4) | ((uint16_t)a[11 * i + 6] << 4)) & 0x7FF;
        r->coeffs[8 * i + 5] = ((a[11 * i + 6] >> 7) | ((uint16_t)a[11 * i + 7] << 1) | ((uint16_t)a[11 * i + 8] << 9)) & 0x7FF;
        r->coeffs[8 * i + 6] = ((a[11 * i + 8] >> 2) | ((uint16_t)a[11 * i + 9] << 6)) & 0x7FF;
        r->coeffs[8 * i + 7] = ((a[11 * i + 9] >> 5) | ((uint16_t)a[11 * i + 10] << 3)) & 0x7FF;
    }
    r->coeffs[8 * i + 0] = ((a[11 * i + 0] >> 0) | ((uint16_t)a[11 * i + 1] << 8)) & 0x7FF;
    r->coeffs[8 * i + 1] = ((a[11 * i + 1] >> 3) | ((uint16_t)a[11 * i + 2] << 5)) & 0x7FF;
    r->coeffs[8 * i + 2] = ((a[11 * i + 2] >> 6) | ((uint16_t)a[11 * i + 3] << 2) | ((uint16_t)a[11 * i + 4] << 10)) & 0x7FF;
    r->coeffs[8 * i + 3] = ((a[11 * i + 4] >> 1) | ((uint16_t)a[11 * i + 5] << 7)) & 0x7FF;
    r->coeffs[8 * i + 4] = ((a[11 * i + 5] >> 4) | ((uint16_t)a[11 * i + 6] << 4)) & 0x7FF;
#elif (FPTRU_Q2 == 4096)
    for (i = 0; i < FPTRU_N / 2; i++)
    {
        r->coeffs[2 * i] = ((a[3 * i + 0] >> 0) | ((uint16_t)a[3 * i + 1] << 8)) & 0xFFF;
        r->coeffs[2 * i + 1] = ((a[3 * i + 1] >> 4) | ((uint16_t)a[3 * i + 2] << 4)) & 0xFFF;
    }
    r->coeffs[2 * i] = ((a[3 * i + 0] >> 0) | (((uint16_t)a[3 * i + 1] & 0xF) << 8)) & 0xFFF;
#endif
}