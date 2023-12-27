#ifndef FPTRU_H
#define FPTRU_H

#include "poly.h"

void crypto_pke_keygen(unsigned char pk[FPTRU_PKE_PUBLICKEYBYTES],
                       unsigned char sk[FPTRU_PKE_SECRETKEYBYTES],
                       const unsigned char coins[FPTRU_N]);

void crypto_pke_enc(unsigned char ct[FPTRU_PKE_CIPHERTEXTBYTES],
                    const unsigned char pk[FPTRU_PKE_PUBLICKEYBYTES],
                    const unsigned char m[FPTRU_MSGBYTES],
                    const unsigned char coins[FPTRU_N]);

void crypto_pke_dec(unsigned char m[FPTRU_MSGBYTES],
                    const unsigned char ct[FPTRU_PKE_CIPHERTEXTBYTES],
                    const unsigned char sk[FPTRU_PKE_SECRETKEYBYTES]);

#endif
