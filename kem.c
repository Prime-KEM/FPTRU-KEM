#include <stddef.h>
#include "randombytes.h"
#include "params.h"
#include "pke.h"
#include "poly.h"
#include "pack.h"
#include "symmetric_crypto.h"

int crypto_kem_keygen(unsigned char *pk,
                      unsigned char *sk)
{
  unsigned int i;
  unsigned char coins[FPTRU_COIN_BYTES];

  randombytes(coins, FPTRU_SEEDBYTES);
  crypto_hash_shake256(coins, FPTRU_COIN_BYTES, coins, FPTRU_SEEDBYTES);
  crypto_pke_keygen(pk, sk, coins);

  for (i = 0; i < FPTRU_PKE_PUBLICKEYBYTES; ++i)
    sk[i + FPTRU_PKE_SECRETKEYBYTES] = pk[i];

  randombytes(sk + FPTRU_PKE_SECRETKEYBYTES + FPTRU_PKE_PUBLICKEYBYTES, FPTRU_SEEDBYTES);

  return 0;
}

int crypto_kem_encaps(unsigned char *ct,
                      unsigned char *k,
                      const unsigned char *pk)
{
  unsigned int i;
  unsigned char buf[FPTRU_SHAREDKEYBYTES + FPTRU_COIN_BYTES / 2], m[FPTRU_PREFIXHASHBYTES + FPTRU_MSGBYTES];

  randombytes(buf, FPTRU_SEEDBYTES);
  crypto_hash_shake256(m + FPTRU_PREFIXHASHBYTES, FPTRU_MSGBYTES, buf, 32);

  for (i = 0; i < FPTRU_PREFIXHASHBYTES; ++i)
    m[i] = pk[i];

  crypto_hash_sha3_512(buf, m, FPTRU_PREFIXHASHBYTES + FPTRU_MSGBYTES);
  crypto_hash_shake256(buf + FPTRU_SHAREDKEYBYTES, FPTRU_COIN_BYTES / 2, buf + FPTRU_SHAREDKEYBYTES, 32);

  crypto_pke_enc(ct, pk, m + FPTRU_PREFIXHASHBYTES, buf + FPTRU_SHAREDKEYBYTES);

  for (i = 0; i < FPTRU_SHAREDKEYBYTES; ++i)
    k[i] = buf[i];

  return 0;
}

int crypto_kem_decaps(unsigned char *k,
                      const unsigned char *ct,
                      const unsigned char *sk)
{
  unsigned int i;
  unsigned char buf[FPTRU_SHAREDKEYBYTES + FPTRU_COIN_BYTES / 2], buf2[FPTRU_SHAREDKEYBYTES * 2], m[FPTRU_PREFIXHASHBYTES + FPTRU_MSGBYTES];
  unsigned char ct2[FPTRU_PKE_CIPHERTEXTBYTES + FPTRU_SEEDBYTES + FPTRU_PREFIXHASHBYTES];
  int16_t t;
  int32_t fail;

  crypto_pke_dec(m + FPTRU_PREFIXHASHBYTES, ct, sk);

  for (i = 0; i < FPTRU_PREFIXHASHBYTES; ++i)
    m[i] = sk[i + FPTRU_PKE_SECRETKEYBYTES];

  crypto_hash_sha3_512(buf, m, FPTRU_PREFIXHASHBYTES + FPTRU_MSGBYTES);
  crypto_hash_shake256(buf + FPTRU_SHAREDKEYBYTES, FPTRU_COIN_BYTES / 2, buf + FPTRU_SHAREDKEYBYTES, 32);
  crypto_pke_enc(ct2, sk + FPTRU_PKE_SECRETKEYBYTES, m + FPTRU_PREFIXHASHBYTES, buf + FPTRU_SHAREDKEYBYTES);

  t = 0;
  for (i = 0; i < FPTRU_PKE_CIPHERTEXTBYTES; ++i)
    t |= ct[i] ^ ct2[i];

  fail = (uint16_t)t;
  fail = (-fail) >> 31;

  for (i = 0; i < FPTRU_PREFIXHASHBYTES; ++i)
    ct2[i] = sk[i + FPTRU_PKE_SECRETKEYBYTES];
  for (i = 0; i < FPTRU_SEEDBYTES; ++i)
    ct2[i + FPTRU_PREFIXHASHBYTES] = sk[i + FPTRU_PKE_SECRETKEYBYTES + FPTRU_PKE_PUBLICKEYBYTES];
  for (i = 0; i < FPTRU_PKE_CIPHERTEXTBYTES; ++i)
    ct2[i + FPTRU_PREFIXHASHBYTES + FPTRU_SEEDBYTES] = ct[i];
  crypto_hash_sha3_512(buf2, ct2, FPTRU_PKE_CIPHERTEXTBYTES + FPTRU_SEEDBYTES + FPTRU_PREFIXHASHBYTES);
  for (i = 0; i < FPTRU_SHAREDKEYBYTES; ++i)
    k[i] = buf[i] ^ ((-fail) & (buf[i] ^ buf2[i]));

  return fail;
}
