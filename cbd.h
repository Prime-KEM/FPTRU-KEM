#ifndef CBD_H
#define CBD_H

#include <stdint.h>
#include "params.h"
#include "poly.h"

void cbd2(poly *r, const uint8_t buf[FPTRU_N / 2]);
void cbd3(poly *r, const uint8_t buf[FPTRU_N * 3 / 4]);

#endif
