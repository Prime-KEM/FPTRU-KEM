#include <stdint.h>
#include "params.h"
#include "cbd.h"

#if (FPTRU_BOUND == 5)
static uint32_t load32_littleendian(const uint8_t x[4])
{
  uint32_t r;
  r = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  r |= (uint32_t)x[3] << 24;
  return r;
}

void cbd2(poly *r, const uint8_t buf[FPTRU_COIN_BYTES / 2])
{
  unsigned int i, j;
  uint32_t t, d;
  int16_t a, b;

#if (FPTRU_N == 761)
  for (i = 0; i < FPTRU_N / 8; i++)
  {
    t = load32_littleendian(buf + 4 * i);
    d = t & 0x55555555;
    d += (t >> 1) & 0x55555555;

    for (j = 0; j < 8; j++)
    {
      a = (d >> (4 * j + 0)) & 0x3;
      b = (d >> (4 * j + 2)) & 0x3;
      r->coeffs[8 * i + j] = a - b;
    }
  }
  t = buf[4 * i];
  a = 0;
  b = 0;
  for (j = 0; j < 2; j++)
    a += (t >> j) & 0x1;
  for (j = 2; j < 4; j++)
    b += (t >> j) & 0x1;
  r->coeffs[8 * i + 0] = (int16_t)(a - b);

#elif (FPTRU_N == 1277)
  for (i = 0; i < FPTRU_N / 8; i++)
  {
    t = load32_littleendian(buf + 4 * i);
    d = t & 0x55555555;
    d += (t >> 1) & 0x55555555;

    for (j = 0; j < 8; j++)
    {
      a = (d >> (4 * j + 0)) & 0x3;
      b = (d >> (4 * j + 2)) & 0x3;
      r->coeffs[8 * i + j] = a - b;
    }
  }

  for (int k = 0; k < 5; k++)
  {
    t = (buf[4 * i + (k >> 1)] >> ((k & 1) << 2)) & 0xf;
    a = 0;
    b = 0;
    for (j = 0; j < 2; j++)
      a += (t >> j) & 0x1;
    for (j = 2; j < 4; j++)
      b += (t >> j) & 0x1;
    r->coeffs[8 * i + k] = (int16_t)(a - b);
  }
#endif
}
#endif

#if (FPTRU_BOUND == 7)
static uint64_t load_littleendian(const uint8_t *x, int bytes)
{
  int i;
  uint64_t r = x[0];
  for (i = 1; i < bytes; i++)
    r |= (uint64_t)x[i] << (8 * i);
  return r;
}

void cbd3(poly *r, const uint8_t buf[FPTRU_COIN_BYTES / 2])
{
  uint32_t t, d, a[4], b[4];
  int i, j;

#if (FPTRU_N == 653)
  for (i = 0; i < FPTRU_N / 4; i++)
  {
    t = load_littleendian(buf + 3 * i, 3);
    d = 0;
    for (j = 0; j < 3; j++)
      d += (t >> j) & 0x249249;

    a[0] = d & 0x7;
    b[0] = (d >> 3) & 0x7;
    a[1] = (d >> 6) & 0x7;
    b[1] = (d >> 9) & 0x7;
    a[2] = (d >> 12) & 0x7;
    b[2] = (d >> 15) & 0x7;
    a[3] = (d >> 18) & 0x7;
    b[3] = (d >> 21);

    r->coeffs[4 * i + 0] = (int16_t)(a[0] - b[0]);
    r->coeffs[4 * i + 1] = (int16_t)(a[1] - b[1]);
    r->coeffs[4 * i + 2] = (int16_t)(a[2] - b[2]);
    r->coeffs[4 * i + 3] = (int16_t)(a[3] - b[3]);
  }
  t = buf[3 * i];
  a[0] = 0;
  b[0] = 0;
  for (j = 0; j < 3; j++)
    a[0] += (t >> j) & 0x1;
  for (j = 3; j < 6; j++)
    b[0] += (t >> j) & 0x1;
  r->coeffs[4 * i + 0] = (int16_t)(a[0] - b[0]);

#elif (FPTRU_N == 761)
  for (i = 0; i < FPTRU_N / 4; i++)
  {
    t = load_littleendian(buf + 3 * i, 3);
    d = 0;
    for (j = 0; j < 3; j++)
      d += (t >> j) & 0x249249;

    a[0] = d & 0x7;
    b[0] = (d >> 3) & 0x7;
    a[1] = (d >> 6) & 0x7;
    b[1] = (d >> 9) & 0x7;
    a[2] = (d >> 12) & 0x7;
    b[2] = (d >> 15) & 0x7;
    a[3] = (d >> 18) & 0x7;
    b[3] = (d >> 21);

    r->coeffs[4 * i + 0] = (int16_t)(a[0] - b[0]);
    r->coeffs[4 * i + 1] = (int16_t)(a[1] - b[1]);
    r->coeffs[4 * i + 2] = (int16_t)(a[2] - b[2]);
    r->coeffs[4 * i + 3] = (int16_t)(a[3] - b[3]);
  }
  t = buf[3 * i];
  a[0] = 0;
  b[0] = 0;
  for (j = 0; j < 3; j++)
    a[0] += (t >> j) & 0x1;
  for (j = 3; j < 6; j++)
    b[0] += (t >> j) & 0x1;
  r->coeffs[4 * i + 0] = (int16_t)(a[0] - b[0]);
#endif
}
#endif