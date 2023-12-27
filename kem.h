#ifndef KEM_H
#define KEM_H

#include "params.h"

int crypto_kem_keygen(unsigned char *pk, unsigned char *sk);
int crypto_kem_encaps(unsigned char *ct,
                      unsigned char *k,
                      const unsigned char *pk);
int crypto_kem_decaps(unsigned char *k,
                      const unsigned char *ct,
                      const unsigned char *sk);

#endif
