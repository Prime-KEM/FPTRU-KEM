#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "params.h"
#include "kem.h"

#define NTESTS 10000

void test_kem()
{
  unsigned int i, j;
  unsigned char k1[FPTRU_SHAREDKEYBYTES], k2[FPTRU_SHAREDKEYBYTES];
  unsigned char pk[FPTRU_KEM_PUBLICKEYBYTES] = {0}, sk[FPTRU_KEM_SECRETKEYBYTES] = {0};
  unsigned char ct[FPTRU_KEM_CIPHERTEXTBYTES] = {0};

  for (i = 0; i < NTESTS; i++)
  {
    crypto_kem_keygen(pk, sk);
    crypto_kem_encaps(ct, k1, pk);
    crypto_kem_decaps(k2, ct, sk);

    for (j = 0; j < FPTRU_SHAREDKEYBYTES; j++)
      if (k1[j] != k2[j])
      {
        printf("Round %d. Failure: Keys dont match: %hhx != %hhx!\n", i, k1[j], k2[j]);
        return;
      }
  }

  printf("FPTRU-%d-KEM is correct!\n", FPTRU_N);

  printf("Test %d times.\n\n", NTESTS);
  printf("FPTRU_N = %d, FPTRU_Q = %d, FPTRU_Q2 = %d\n", FPTRU_N, FPTRU_Q, FPTRU_Q2);
  printf("KEM size: pk = %d bytes, ct = %d bytes, bandwidth = %d bytes\n\n",
         FPTRU_KEM_PUBLICKEYBYTES, FPTRU_KEM_CIPHERTEXTBYTES,
         FPTRU_KEM_PUBLICKEYBYTES + FPTRU_KEM_CIPHERTEXTBYTES);
}

int main()
{
  test_kem();
  return 0;
}
