#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "kem.h"
#include "params.h"
#include "pke.h"
#include "poly.h"
#include "cpucycles.h"
#include "speed.h"
#include "randombytes.h"

#define NTESTS 10000

uint64_t t[NTESTS];

void test_speed_kem()
{
  printf("\n");

  printf("FPTRU-%d-%d-KEM\n\n", FPTRU_N, FPTRU_Q);

  unsigned int i;
  unsigned char pk[FPTRU_KEM_PUBLICKEYBYTES], sk[FPTRU_KEM_SECRETKEYBYTES], ct[FPTRU_KEM_CIPHERTEXTBYTES];
  unsigned char k1[FPTRU_SHAREDKEYBYTES], k2[FPTRU_SHAREDKEYBYTES];

  for (i = 0; i < NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_keygen(pk, sk);
  }
  print_results("FPTRU_kem_keygen: ", t, NTESTS);

  for (i = 0; i < NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_encaps(ct, k1, pk);
  }
  print_results("FPTRU_kem_encaps: ", t, NTESTS);

  for (i = 0; i < NTESTS; i++)
  {
    t[i] = cpucycles();
    crypto_kem_decaps(k2, ct, sk);
  }
  print_results("FPTRU_kem_decaps: ", t, NTESTS);
}

int main()
{
  test_speed_kem();
  return 0;
}
