#include "xoshiro128.h"

inline  uint32_t rotl(const uint32_t x, int k) {
	return (x << k) | (x >> (32 - k));
}

uint32_t next(uint32_t *s) {
	const uint32_t result = rotl(s[1] * 5, 7) * 9;

	const uint32_t t = s[1] << 9;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 11);

	return result;
}

void jump(uint32_t *s) {
	static const uint32_t JUMP[] = { 0x8764000b, 0xf542d2d3, 0x6fa035c3, 0x77f2db5b };

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;
	for(int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
		for(int b = 0; b < 32; b++) {
			if (JUMP[i] & UINT32_C(1) << b) {
				s0 ^= s[0];
				s1 ^= s[1];
				s2 ^= s[2];
				s3 ^= s[3];
			}
			next(s);
		}

	s[0] = s0;
	s[1] = s1;
	s[2] = s2;
	s[3] = s3;
}

void long_jump(uint32_t *s) {
	static const uint32_t LONG_JUMP[] = { 0xb523952e, 0x0b6f099f, 0xccf5a0ef, 0x1c580662 };

	uint32_t s0 = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t s3 = 0;
	for(int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for(int b = 0; b < 32; b++) {
			if (LONG_JUMP[i] & UINT32_C(1) << b) {
				s0 ^= s[0];
				s1 ^= s[1];
				s2 ^= s[2];
				s3 ^= s[3];
			}
			next(s);
		}

	s[0] = s0;
	s[1] = s1;
	s[2] = s2;
	s[3] = s3;
}
