#include <stdio.h>
#include "params.h"
#include "poly.h"
#include "pack.h"
#include "inverse.h"
#if (FPTRU_N == 653)
#include "poly_mul_n653q/radix_ntt_n653.h"
#elif (FPTRU_N == 761)
#include "poly_mul_n761q/radix_ntt_n761.h"
#elif (FPTRU_N == 1277)
#include "poly_mul_n1277q/radix_ntt_n1277.h"
#endif

void crypto_pke_keygen(unsigned char pk[FPTRU_PKE_PUBLICKEYBYTES],
                       unsigned char sk[FPTRU_PKE_SECRETKEYBYTES],
                       const unsigned char coins[FPTRU_COIN_BYTES])
{
  poly f, finv, g, h;

  poly_sample(&f, coins);
  poly_sample(&g, coins + FPTRU_COIN_BYTES / 2);

  poly_double(&f, &f);
  f.coeffs[0] += 1;

  poly_inverse(&finv, &f);
  poly_mul_q1(&h, &finv, &g);
  poly_fqcsubq(&h);

  pack_pk(pk, &h);
  pack_sk(sk, &f);
}

void crypto_pke_enc(unsigned char ct[FPTRU_PKE_CIPHERTEXTBYTES],
                    const unsigned char pk[FPTRU_PKE_PUBLICKEYBYTES],
                    const unsigned char m[FPTRU_MSGBYTES],
                    const unsigned char coins[FPTRU_COIN_BYTES / 2])
{

  poly h, r, sigma, c;

  unpack_pk(&h, pk);

  poly_sample(&r, coins);
  poly_mul_q1(&sigma, &h, &r);
  poly_fqcsubq(&sigma);

  poly_encode_compress(&c, &sigma, m);

  pack_ct(ct, &c);
}

void crypto_pke_dec(unsigned char m[FPTRU_MSGBYTES],
                    const unsigned char ct[FPTRU_PKE_CIPHERTEXTBYTES],
                    const unsigned char sk[FPTRU_PKE_SECRETKEYBYTES])
{
  poly c, f;

  unpack_ct(&c, ct);
  unpack_sk(&f, sk);

  poly_decode(m, &c, &f);
}
