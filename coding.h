#ifndef CODING_H
#define CODING_H

#include <stdint.h>
#include "params.h"

uint8_t encode_e8(uint8_t m);
uint8_t decode_e8(uint32_t vec[8]);

#endif