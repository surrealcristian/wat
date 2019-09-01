#ifndef XOSHIRO256_H
#define XOSHIRO256_H

#include <inttypes.h>

uint32_t rotl(const uint32_t x, int k);
uint32_t next(uint32_t *s);

void jump(uint32_t *s);
void long_jump(uint32_t *s);

#endif // XOSHIRO256_H

