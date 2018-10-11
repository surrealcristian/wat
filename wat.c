#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"


// NAV_Config
#define WINDOW_W 480
#define WINDOW_H 640

#define UPDATES_PER_SECOND 120.0
#define MS_PER_UPDATE 8.33 // 1000 / 120.0
#define SLEEP_MS 2.08 // 1000 / 480.0

#define STATE_IN_GAME 0
#define STATE_PAUSE 1

#define PLAYER_W 16
#define PLAYER_H 16
#define PLAYER_V 512

#define PLAYER_BULLETS_MAX 128
#define PLAYER_BULLETS_INIT_N 5
#define PLAYER_BULLETS_W 4
#define PLAYER_BULLETS_H 4
#define PLAYER_BULLETS_V 2048
#define PLAYER_BULLETS_VX +0
#define PLAYER_BULLETS_VY -1

#define ENEMIES_MAX 16
#define ENEMY_W 32
#define ENEMY_H 32
#define ENEMY_V 128
#define ENEMY_VX +0
#define ENEMY_VY +1

#define PARTICLES_MAX 128
#define PARTICLE_W 8
#define PARTICLE_H 8
#define PARTICLE_V 1024

#define EXPLOSION_PARTICLES_N 4

float PLAYER_BULLETS_OFFSET_X[5] = { +0.00, -0.75, +0.75, -1.50, +1.50 };
float PLAYER_BULLETS_OFFSET_Y[5] = { -1.00, -0.50, -0.50, +0.00, +0.00 };

float EXPLOSION_PARTICLES_VX[4] = { -1.00, +1.00, -1.00, +1.00 };
float EXPLOSION_PARTICLES_VY[4] = { -1.00, -1.00, +1.00, +1.00 };

float CHARACTERS_DATA_POSITION_X[25] = { 0, 2, 4, 6, 8,    0, 2, 4, 6, 8,    0, 2, 4, 6, 8,    0, 2, 4, 6, 8,    0, 2, 4, 6, 8, };
float CHARACTERS_DATA_POSITION_Y[25] = { 0, 0, 0, 0, 0,    2, 2, 2, 2, 2,    4, 4, 4, 4, 4,    6, 6, 6, 6, 6,    8, 8, 8, 8, 8, };

int CHARACTERS_DATA[36][25] = {
    /* 0 */ { 0, 1, 1, 1, 0,    1, 0, 0, 1, 1,    1, 0, 1, 0, 1,    1, 1, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 1 */ { 0, 0, 1, 0, 0,    1, 1, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* 2 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* 3 */ { 1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    0, 0, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 4 */ { 0, 0, 0, 1, 0,    0, 0, 1, 1, 0,    1, 1, 0, 1, 0,    1, 1, 1, 1, 1,    0, 0, 0, 1, 0, },
    /* 5 */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* 6 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 7 */ { 1, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0, },
    /* 8 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* 9 */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* A */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* B */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* C */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 0,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* D */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* E */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* F */ { 1, 1, 1, 1, 1,    1, 0, 0, 0, 0,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* G */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* H */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 1, 1, 1, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1, },
    /* I */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    1, 1, 1, 1, 1, },
    /* J */ { 0, 1, 1, 1, 1,    0, 0, 0, 0, 1,    0, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },

    /* K */ { 1, 0, 0, 0, 1,    1, 0, 0, 1, 0,    1, 1, 1, 0, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* L */ { 1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0,    1, 1, 1, 1, 1, },
    /* M */ { 1, 0, 0, 0, 1,    1, 1, 0, 1, 1,    1, 0, 1, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 0, 1, },
    /* N */ { 1, 0, 0, 0, 1,    1, 1, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* O */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* P */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 0, 0,    1, 0, 0, 0, 0, },
    /* Q */ { 0, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 0, 0, 1, 0,    0, 1, 1, 0, 1, },
    /* R */ { 1, 1, 1, 1, 0,    1, 0, 0, 0, 1,    1, 1, 1, 1, 0,    1, 0, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* S */ { 0, 1, 1, 1, 1,    1, 0, 0, 0, 0,    0, 1, 1, 1, 0,    0, 0, 0, 0, 1,    1, 1, 1, 1, 0, },
    /* T */ { 1, 1, 1, 1, 1,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },

    /* U */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 1, 1, 0, },
    /* V */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0, },
    /* W */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    1, 0, 1, 0, 1,    1, 1, 0, 1, 1,    1, 0, 0, 0, 1, },
    /* X */ { 1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 1, 0,    1, 0, 0, 0, 1, },
    /* Y */ { 1, 0, 0, 0, 1,    1, 0, 0, 0, 1,    0, 1, 0, 1, 0,    0, 0, 1, 0, 0,    0, 0, 1, 0, 0, },
    /* Z */ { 1, 1, 1, 1, 1,    0, 0, 0, 1, 0,    0, 0, 1, 0, 0,    0, 1, 0, 0, 0,    1, 1, 1, 1, 1, },
};
// END NAV_Config


// NAV_Libs

// NAV_TinyMT

// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/VERSIONS/C-LANG/c-lang.html
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html

// version 1.1.1

#ifndef TINYMT32_H
#define TINYMT32_H
/**
 * @file tinymt32.h
 *
 * @brief Tiny Mersenne Twister only 127 bit internal state
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (University of Tokyo)
 *
 * Copyright (C) 2011 Mutsuo Saito, Makoto Matsumoto,
 * Hiroshima University and The University of Tokyo.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */

#include <stdint.h>
#include <inttypes.h>

#define TINYMT32_MEXP 127
#define TINYMT32_SH0 1
#define TINYMT32_SH1 10
#define TINYMT32_SH8 8
#define TINYMT32_MASK UINT32_C(0x7fffffff)
#define TINYMT32_MUL (1.0f / 16777216.0f)

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * tinymt32 internal state vector and parameters
 */
struct TINYMT32_T {
    uint32_t status[4];
    uint32_t mat1;
    uint32_t mat2;
    uint32_t tmat;
};

typedef struct TINYMT32_T tinymt32_t;

void tinymt32_init(tinymt32_t * random, uint32_t seed);
void tinymt32_init_by_array(tinymt32_t * random, uint32_t init_key[],
			    int key_length);

#if defined(__GNUC__)
/**
 * This function always returns 127
 * @param random not used
 * @return always 127
 */
inline static int tinymt32_get_mexp(
    tinymt32_t * random  __attribute__((unused))) {
    return TINYMT32_MEXP;
}
#else
inline static int tinymt32_get_mexp(tinymt32_t * random) {
    return TINYMT32_MEXP;
}
#endif

/**
 * This function changes internal state of tinymt32.
 * Users should not call this function directly.
 * @param random tinymt internal status
 */
inline static void tinymt32_next_state(tinymt32_t * random) {
    uint32_t x;
    uint32_t y;

    y = random->status[3];
    x = (random->status[0] & TINYMT32_MASK)
	^ random->status[1]
	^ random->status[2];
    x ^= (x << TINYMT32_SH0);
    y ^= (y >> TINYMT32_SH0) ^ x;
    random->status[0] = random->status[1];
    random->status[1] = random->status[2];
    random->status[2] = x ^ (y << TINYMT32_SH1);
    random->status[3] = y;
    random->status[1] ^= -((int32_t)(y & 1)) & random->mat1;
    random->status[2] ^= -((int32_t)(y & 1)) & random->mat2;
}

/**
 * This function outputs 32-bit unsigned integer from internal state.
 * Users should not call this function directly.
 * @param random tinymt internal status
 * @return 32-bit unsigned pseudorandom number
 */
inline static uint32_t tinymt32_temper(tinymt32_t * random) {
    uint32_t t0, t1;
    t0 = random->status[3];
#if defined(LINEARITY_CHECK)
    t1 = random->status[0]
	^ (random->status[2] >> TINYMT32_SH8);
#else
    t1 = random->status[0]
	+ (random->status[2] >> TINYMT32_SH8);
#endif
    t0 ^= t1;
    t0 ^= -((int32_t)(t1 & 1)) & random->tmat;
    return t0;
}

/**
 * This function outputs floating point number from internal state.
 * Users should not call this function directly.
 * @param random tinymt internal status
 * @return floating point number r (1.0 <= r < 2.0)
 */
inline static float tinymt32_temper_conv(tinymt32_t * random) {
    uint32_t t0, t1;
    union {
	uint32_t u;
	float f;
    } conv;

    t0 = random->status[3];
#if defined(LINEARITY_CHECK)
    t1 = random->status[0]
	^ (random->status[2] >> TINYMT32_SH8);
#else
    t1 = random->status[0]
	+ (random->status[2] >> TINYMT32_SH8);
#endif
    t0 ^= t1;
    conv.u = ((t0 ^ (-((int32_t)(t1 & 1)) & random->tmat)) >> 9)
	      | UINT32_C(0x3f800000);
    return conv.f;
}

/**
 * This function outputs floating point number from internal state.
 * Users should not call this function directly.
 * @param random tinymt internal status
 * @return floating point number r (1.0 < r < 2.0)
 */
inline static float tinymt32_temper_conv_open(tinymt32_t * random) {
    uint32_t t0, t1;
    union {
	uint32_t u;
	float f;
    } conv;

    t0 = random->status[3];
#if defined(LINEARITY_CHECK)
    t1 = random->status[0]
	^ (random->status[2] >> TINYMT32_SH8);
#else
    t1 = random->status[0]
	+ (random->status[2] >> TINYMT32_SH8);
#endif
    t0 ^= t1;
    conv.u = ((t0 ^ (-((int32_t)(t1 & 1)) & random->tmat)) >> 9)
	      | UINT32_C(0x3f800001);
    return conv.f;
}

/**
 * This function outputs 32-bit unsigned integer from internal state.
 * @param random tinymt internal status
 * @return 32-bit unsigned integer r (0 <= r < 2^32)
 */
inline static uint32_t tinymt32_generate_uint32(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper(random);
}

/**
 * This function outputs floating point number from internal state.
 * This function is implemented using multiplying by (1 / 2^24).
 * floating point multiplication is faster than using union trick in
 * my Intel CPU.
 * @param random tinymt internal status
 * @return floating point number r (0.0 <= r < 1.0)
 */
inline static float tinymt32_generate_float(tinymt32_t * random) {
    tinymt32_next_state(random);
    return (tinymt32_temper(random) >> 8) * TINYMT32_MUL;
}

/**
 * This function outputs floating point number from internal state.
 * This function is implemented using union trick.
 * @param random tinymt internal status
 * @return floating point number r (1.0 <= r < 2.0)
 */
inline static float tinymt32_generate_float12(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper_conv(random);
}

/**
 * This function outputs floating point number from internal state.
 * This function is implemented using union trick.
 * @param random tinymt internal status
 * @return floating point number r (0.0 <= r < 1.0)
 */
inline static float tinymt32_generate_float01(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper_conv(random) - 1.0f;
}

/**
 * This function outputs floating point number from internal state.
 * This function may return 1.0 and never returns 0.0.
 * @param random tinymt internal status
 * @return floating point number r (0.0 < r <= 1.0)
 */
inline static float tinymt32_generate_floatOC(tinymt32_t * random) {
    tinymt32_next_state(random);
    return 1.0f - tinymt32_generate_float(random);
}

/**
 * This function outputs floating point number from internal state.
 * This function returns neither 0.0 nor 1.0.
 * @param random tinymt internal status
 * @return floating point number r (0.0 < r < 1.0)
 */
inline static float tinymt32_generate_floatOO(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper_conv_open(random) - 1.0f;
}

/**
 * This function outputs double precision floating point number from
 * internal state. The returned value has 32-bit precision.
 * In other words, this function makes one double precision floating point
 * number from one 32-bit unsigned integer.
 * @param random tinymt internal status
 * @return floating point number r (0.0 < r <= 1.0)
 */
inline static double tinymt32_generate_32double(tinymt32_t * random) {
    tinymt32_next_state(random);
    return tinymt32_temper(random) * (1.0 / 4294967296.0);
}

#if defined(__cplusplus)
}
#endif

#endif


/**
 * @file tinymt32.c
 *
 * @brief Tiny Mersenne Twister only 127 bit internal state
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (The University of Tokyo)
 *
 * Copyright (C) 2011 Mutsuo Saito, Makoto Matsumoto,
 * Hiroshima University and The University of Tokyo.
 * All rights reserved.
 *
 * The 3-clause BSD License is applied to this software, see
 * LICENSE.txt
 */
//#include "tinymt32.h"
#define MIN_LOOP 8
#define PRE_LOOP 8

/**
 * This function represents a function used in the initialization
 * by init_by_array
 * @param x 32-bit integer
 * @return 32-bit integer
 */
static uint32_t ini_func1(uint32_t x) {
    return (x ^ (x >> 27)) * UINT32_C(1664525);
}

/**
 * This function represents a function used in the initialization
 * by init_by_array
 * @param x 32-bit integer
 * @return 32-bit integer
 */
static uint32_t ini_func2(uint32_t x) {
    return (x ^ (x >> 27)) * UINT32_C(1566083941);
}

/**
 * This function certificate the period of 2^127-1.
 * @param random tinymt state vector.
 */
static void period_certification(tinymt32_t * random) {
    if ((random->status[0] & TINYMT32_MASK) == 0 &&
	random->status[1] == 0 &&
	random->status[2] == 0 &&
	random->status[3] == 0) {
	random->status[0] = 'T';
	random->status[1] = 'I';
	random->status[2] = 'N';
	random->status[3] = 'Y';
    }
}

/**
 * This function initializes the internal state array with a 32-bit
 * unsigned integer seed.
 * @param random tinymt state vector.
 * @param seed a 32-bit unsigned integer used as a seed.
 */
void tinymt32_init(tinymt32_t * random, uint32_t seed) {
    random->status[0] = seed;
    random->status[1] = random->mat1;
    random->status[2] = random->mat2;
    random->status[3] = random->tmat;
    for (int i = 1; i < MIN_LOOP; i++) {
	random->status[i & 3] ^= i + UINT32_C(1812433253)
	    * (random->status[(i - 1) & 3]
	       ^ (random->status[(i - 1) & 3] >> 30));
    }
    period_certification(random);
    for (int i = 0; i < PRE_LOOP; i++) {
	tinymt32_next_state(random);
    }
}

/**
 * This function initializes the internal state array,
 * with an array of 32-bit unsigned integers used as seeds
 * @param random tinymt state vector.
 * @param init_key the array of 32-bit integers, used as a seed.
 * @param key_length the length of init_key.
 */
void tinymt32_init_by_array(tinymt32_t * random, uint32_t init_key[],
			    int key_length) {
    const int lag = 1;
    const int mid = 1;
    const int size = 4;
    int i, j;
    int count;
    uint32_t r;
    uint32_t * st = &random->status[0];

    st[0] = 0;
    st[1] = random->mat1;
    st[2] = random->mat2;
    st[3] = random->tmat;
    if (key_length + 1 > MIN_LOOP) {
	count = key_length + 1;
    } else {
	count = MIN_LOOP;
    }
    r = ini_func1(st[0] ^ st[mid % size]
		  ^ st[(size - 1) % size]);
    st[mid % size] += r;
    r += key_length;
    st[(mid + lag) % size] += r;
    st[0] = r;
    count--;
    for (i = 1, j = 0; (j < count) && (j < key_length); j++) {
	r = ini_func1(st[i % size]
		      ^ st[(i + mid) % size]
		      ^ st[(i + size - 1) % size]);
	st[(i + mid) % size] += r;
	r += init_key[j] + i;
	st[(i + mid + lag) % size] += r;
	st[i % size] = r;
	i = (i + 1) % size;
    }
    for (; j < count; j++) {
	r = ini_func1(st[i % size]
		      ^ st[(i + mid) % size]
		      ^ st[(i + size - 1) % size]);
	st[(i + mid) % size] += r;
	r += i;
	st[(i + mid + lag) % size] += r;
	st[i % size] = r;
	i = (i + 1) % size;
    }
    for (j = 0; j < size; j++) {
	r = ini_func2(st[i % size]
		      + st[(i + mid) % size]
		      + st[(i + size - 1) % size]);
	st[(i + mid) % size] ^= r;
	r -= i;
	st[(i + mid + lag) % size] ^= r;
	st[i % size] = r;
	i = (i + 1) % size;
    }
    period_certification(random);
    for (i = 0; i < PRE_LOOP; i++) {
	tinymt32_next_state(random);
    }
}
// END NAV_TinyMT

// END NAV_Libs


// NAV_Declarations

// NAV_Text
struct Text {
    SDL_Rect rect;
};

void text_init(struct Text *text);
void text_render_character(struct Text *text, SDL_Renderer *renderer, int character, float x, float y);
void text_render(struct Text *self, SDL_Renderer *renderer, char *text, float x, float y);
// END NAV_Text

// NAV_Keys
struct Keys {
    int left;
    int right;
    int up;
    int down;
    int z;
};
// END NAV_Keys


// NAV_Player
struct Player {
    struct Keys *keys;
    struct BulletManager *bullet_manager;
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    int bullets_n;
    int firing;
    int fire_spacing;
    float fire_time;
    SDL_Rect rect;
};

void player_init(struct Player *self, struct Keys *keys, struct BulletManager *bullet_manager, float x, float y, int w, int h, int v);
void player_set_x(struct Player *self, float value);
void player_set_y(struct Player *self, float value);
void player_on_button_a_keydown(struct Player *self);
void player_on_button_a_keyup(struct Player *self);
void player_fire(struct Player *self);
void player_update(struct Player *self);
void player_render(struct Player *self, SDL_Renderer *renderer);
// END NAV_Player


// NAV_Enemy
struct Enemy {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void enemy_init(struct Enemy *self, float x, float y, int w, int h, int v);
void enemy_set_x(struct Enemy *self, float value);
void enemy_set_y(struct Enemy *self, float value);
void enemy_update(struct Enemy *self);
void enemy_render(struct Enemy *self, SDL_Renderer *renderer);
// END NAV_Enemy


// NAV_EnemyManager
struct EnemyManager {
    struct Enemy *enemies;
    int n;
    float time;
    float spacing;
};

void enemy_manager_init(struct EnemyManager *self, struct Enemy *enemies, int n, int w, int h, int v);
struct Enemy *enemy_manager_get_free(struct EnemyManager *self);
void enemy_manager_spawn(struct EnemyManager *self);
void enemy_manager_update(struct EnemyManager *self);
void enemy_manager_render(struct EnemyManager *self, SDL_Renderer *renderer);
// ENDNAV_EnemyManager

// NAV_Bullet
struct Bullet {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v);
float bullet_get_x(struct Bullet *self);
void bullet_set_x(struct Bullet *self, float value);
float bullet_get_y(struct Bullet *self);
void bullet_set_y(struct Bullet *self, float value);
void bullet_update(struct Bullet *self);
void bullet_render(struct Bullet *self, SDL_Renderer *renderer);
// ENV NAV_Bullet


// NAV_BulletManager
struct BulletManager {
    struct Bullet *bullets;
    int n;
};

void bullet_manager_init(struct BulletManager *self, struct Bullet *bullets, int n, int w, int h, int v);
struct Bullet *bullet_manager_get_free(struct BulletManager *self);
void bullet_manager_update(struct BulletManager *self);
void bullet_manager_render(struct BulletManager *self, SDL_Renderer *renderer);
// END NAV_BulletManager


// NAV_Particle
struct Particle {
    float x;
    float y;
    int w;
    int h;
    int v;
    int vx;
    int vy;
    int alive;
    SDL_Rect rect;
};

void particle_init(struct Particle *self, float x, float y, int w, int h, int v);
void particle_set_x(struct Particle *self, float value);
void particle_set_y(struct Particle *self, float value);
void particle_update(struct Particle *self);
void particle_render(struct Particle *self, SDL_Renderer *renderer);
// END NAV_Particle


// NAV_ParticleManager
struct ParticleManager {
    struct Particle *particles;
    int n;
};

void particle_manager_init(struct ParticleManager *self, struct Particle *particles, int n, int w, int h, int v);
struct Particle *particle_manager_get_free(struct ParticleManager *self);
void particle_manager_update(struct ParticleManager *self);
void particle_manager_render(struct ParticleManager *self, SDL_Renderer *renderer);
// END NAV_ParticleManager


// NAV_CollisionManager
struct CollisionManager {
    struct ParticleManager *particle_manager;
    struct Player *player;
    struct Bullet *player_bullets;
    int bullets_n;
    struct Enemy *enemies;
    int enemies_n;
};

void collision_manager_init(struct CollisionManager *self, struct ParticleManager *particle_manager, struct Player *player, struct Bullet *player_bullets, int bullets_n, struct Enemy *enemies, int enemies_n);
void collision_manager_player_vs_enemies(struct CollisionManager *self);
void collision_manager_enemies_vs_player_bullets(struct CollisionManager *self);
void collision_manager_make_explosion(struct CollisionManager *self, float x, float y);
void collision_manager_update(struct CollisionManager *self);
// END NAV_CollisionManager


// NAV_MiscFunctions
double performance_counters_to_ms(Uint64 start, Uint64 end);

void rand_init(uint32_t seed);
int rand_n(int n);
// END NAV_MiscFunctions


// NAV_InGameState
struct InGameState {
    struct Keys *keys;
    struct Player *player;
    struct Bullet *player_bullets;
    struct BulletManager *player_bullet_manager;
    struct Enemy *enemies;
    struct EnemyManager *enemy_manager;
    struct Particle *particles;
    struct ParticleManager *particle_manager;
    struct CollisionManager *collision_manager;
};

void in_game_state_init(
    struct InGameState *self,
    struct Keys *keys,
    struct Player *player,
    struct Bullet *player_bullets,
    struct BulletManager *player_bullet_manager,
    struct Enemy *enemies,
    struct EnemyManager *enemy_manager,
    struct Particle *particles,
    struct ParticleManager *particle_manager,
    struct CollisionManager *collision_manager
);

void in_game_state_update(struct InGameState *self);
void in_game_state_render(struct InGameState *self, SDL_Renderer *renderer);
// END NAV_InGameState


// NAV_PauseState
struct PauseState {
};

void pause_state_update(struct PauseState *pause_state);
void pause_state_render(struct PauseState *pause_state, SDL_Renderer *renderer);
// END NAV_PauseState


// NAV_Game
struct Game {
    struct InputComponent *input_component;
    struct InGameState *in_game_state;
    int state;
};

void game_init(struct Game *self, struct InGameState *in_game_state);
void game_set_input_component(struct Game *self, struct InputComponent *input_component);
void game_run(struct Game *self, SDL_Renderer *renderer);
// END NAV_Game


// NAV_InputComponent
struct InputComponent {
    struct Keys *keys;
    struct Game *game;
    struct Player *player;
};

void input_component_init(
    struct InputComponent *self,
    struct Keys *keys,
    struct Game *game,
    struct Player *player
);
void input_component_update(struct InputComponent *self, SDL_Event *event);
// END NAV_InputComponent


// NAV_Main
int main(void);
// END NAV_Main

// END NAV_Declarations


// NAV_GlobalVariables
tinymt32_t TINYMT_STATE;
struct Text TEXT;
struct Keys KEYS;
struct InputComponent INPUT_COMPONENT;
struct Player PLAYER;
struct Bullet PLAYER_BULLETS[PLAYER_BULLETS_MAX];
struct BulletManager PLAYER_BULLET_MANAGER;
struct Enemy ENEMIES[ENEMIES_MAX];
struct EnemyManager ENEMY_MANAGER;
struct Particle PARTICLES[PARTICLES_MAX];
struct ParticleManager PARTICLE_MANAGER;
struct CollisionManager COLLISION_MANAGER;
struct InGameState IN_GAME_STATE;
struct Game GAME;
// END NAV_GlobalVariables


// NAV_Code

// NAV_Text
void text_init(struct Text *self) {
    self->rect.w = 2;
    self->rect.h = 2;
}

void text_render_character(struct Text *self, SDL_Renderer *renderer, int character, float x, float y) {
    for (int i = 0; i < 25; i++) {
        if (CHARACTERS_DATA[character][i] == 0) {
            continue;
        }

        self->rect.x = x + CHARACTERS_DATA_POSITION_X[i];
        self->rect.y = y + CHARACTERS_DATA_POSITION_Y[i];

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &self->rect);
    }
}

void text_render(struct Text *self, SDL_Renderer *renderer, char *text, float x, float y) {
    //TODO: complete implementation
    float letter_x = x;
    float letter_y = y;
    int idx;

    for (int i = 0; text[i] != '\0'; i++) {
        int c = text[i];

        if (c >= '0' && c <= '9') {
            idx = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            idx = c - 'A' + 10;
        } else {
            return;
        }

        text_render_character(self, renderer, idx, letter_x, letter_y);

        letter_x += 12;
    }
}
// END NAV_Text

// NAV_Player
void player_init(struct Player *self, struct Keys *keys, struct BulletManager *bullet_manager, float x, float y, int w, int h, int v) {
    self->keys = keys;
    self->bullet_manager = bullet_manager;

    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    player_set_x(self, x);
    player_set_y(self, y);

    self->v = v;

    self->alive = 1;

    self->bullets_n = PLAYER_BULLETS_INIT_N;

    self->fire_spacing = 128;
}

void player_set_x(struct Player *self, float value) {
    float x_min = 0 + self->w / 2;
    float x_max = WINDOW_W - self->w / 2;

    if (value < x_min) {
        value = x_min;
    } else if (value > x_max) {
        value = x_max;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void player_set_y(struct Player *self, float value) {
    float y_min = 0 + self->h / 2;
    float y_max = WINDOW_H - self->h / 2;

    if (value < y_min) {
        value = y_min;
    } else if (value > y_max) {
        value = y_max;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void player_on_button_a_keydown(struct Player *self) {
    self->firing = 1;
    self->fire_time = self->fire_spacing * 1.0;
}

void player_on_button_a_keyup(struct Player *self) {
    self->firing = 0;
    self->fire_time = 0.0;
}

void player_fire(struct Player *self) {
    for (int i = 0; i < self->bullets_n; i++) {
        struct Bullet *bullet = bullet_manager_get_free(self->bullet_manager);

        if (bullet == NULL) {
            return;
        }

        bullet->alive = 1; //TODO: bullet_set_alive()

        bullet_set_x(bullet, self->x + self->w * PLAYER_BULLETS_OFFSET_X[i]);
        bullet_set_y(bullet, self->y + self->h * PLAYER_BULLETS_OFFSET_Y[i]);

        bullet->vx = PLAYER_BULLETS_VX;
        bullet->vy = PLAYER_BULLETS_VY;
    }
}

void player_update(struct Player *self) {
    self->vx = 0;
    self->vy = 0;

    if (self->keys->right) {
        self->vx = +1;
    }

    if (self->keys->left) {
        self->vx = -1;
    }

    if (self->keys->up) {
        self->vy = -1;
    }

    if (self->keys->down) {
        self->vy = +1;
    }

    player_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    player_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));

    if (self->keys->z) {
        self->fire_time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments
    }

    if (self->fire_time >= self->fire_spacing) {
        player_fire(self);
        self->fire_time = 0.0;
    }
}

void player_render(struct Player *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Player

// NAV_Enemy
void enemy_init(struct Enemy *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    enemy_set_x(self, x);
    enemy_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

void enemy_set_x(struct Enemy *self, float value) {
    float x_min = 0 - self->w + 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void enemy_set_y(struct Enemy *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void enemy_update(struct Enemy *self) {
    enemy_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    enemy_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void enemy_render(struct Enemy *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Enemy


// NAV_EnemyManager
void enemy_manager_init(struct EnemyManager *self, struct Enemy *enemies, int n, int w, int h, int v) {
    self->enemies = enemies;
    self->n = n;

    self->time = 0.0;
    self->spacing = 1000.0;

    for (int i = 0; i < self->n; i++) {
        enemy_init(&self->enemies[i], 0, 0, w, h, v);
    }
}

struct Enemy *enemy_manager_get_free(struct EnemyManager *self) {
    struct Enemy *enemy = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 1) {
            continue;
        }

        enemy = &self->enemies[i];

        break;
    }

    return enemy;
}

void enemy_manager_spawn(struct EnemyManager *self) {
    struct Enemy *enemy = enemy_manager_get_free(self);

    if (enemy == NULL) {
        return;
    }

    enemy->alive = 1;

    enemy_set_x(enemy, rand_n(WINDOW_W + 1));
    enemy_set_y(enemy, 0 - enemy->h);

    enemy->vx = ENEMY_VX;
    enemy->vy = ENEMY_VY;
}

void enemy_manager_update(struct EnemyManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        enemy_update(&self->enemies[i]);
    }

    self->time += MS_PER_UPDATE; //TODO: move MS_PER_UPDATE to arguments

    if (self->time >= self->spacing) {
        enemy_manager_spawn(self);
        self->time = 0.0;
    }
}

void enemy_manager_render(struct EnemyManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        enemy_render(&self->enemies[i], renderer);
    }
}
// END NAV_EnemyManager


// NAV_Bullet
void bullet_init(struct Bullet *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    bullet_set_x(self, x);
    bullet_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

float bullet_get_x(struct Bullet *self) {
    return self->x;
}

void bullet_set_x(struct Bullet *self, float value) {
    float x_min = 0 - self->w - 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

float bullet_get_y(struct Bullet *self) {
    return self->y;
}

void bullet_set_y(struct Bullet *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void bullet_update(struct Bullet *self) {
    bullet_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    bullet_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void bullet_render(struct Bullet *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Bullet


// NAV_BulletManager
void bullet_manager_init(struct BulletManager *self, struct Bullet *bullets, int n, int w, int h, int v) {
    self->bullets = bullets;
    self->n = n;

    for (int i = 0; i < n; i++) {
        bullet_init(&bullets[i], 0, 0, w, h, v);
    }
}

struct Bullet *bullet_manager_get_free(struct BulletManager *self) {
    struct Bullet *bullet = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 1) {
            continue;
        }

        bullet = &self->bullets[i];

        break;
    }

    return bullet;
}

void bullet_manager_update(struct BulletManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 0) {
            continue;
        }

        bullet_update(&self->bullets[i]);
    }
}

void bullet_manager_render(struct BulletManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (self->bullets[i].alive == 0) {
            continue;
        }

        bullet_render(&self->bullets[i], renderer);
    }
}
// END NAV_BulletManager


// NAV_Particle
void particle_init(struct Particle *self, float x, float y, int w, int h, int v) {
    self->w = w;
    self->rect.w = self->w;

    self->h = h;
    self->rect.h = self->h;

    particle_set_x(self, x);
    particle_set_y(self, y);

    self->v = v;

    self->alive = 0;
}

void particle_set_x(struct Particle *self, float value) {
    float x_min = 0 - self->w + 1;
    float x_max = WINDOW_W + self->w + 1;

    if (value < x_min || value > x_max) {
        self->alive = 0;
        return;
    }

    self->x = value;

    self->rect.x = floor(self->x - (self->w / 2));
}

void particle_set_y(struct Particle *self, float value) {
    float y_min = 0 - self->h - 1;
    float y_max = WINDOW_H + self->h + 1;

    if (value < y_min || value > y_max) {
        self->alive = 0;
        return;
    }

    self->y = value;

    self->rect.y = floor(self->y - (self->h / 2));
}

void particle_update(struct Particle *self) {
    particle_set_x(self, self->x + (1.0 * self->v * self->vx / UPDATES_PER_SECOND));
    particle_set_y(self, self->y + (1.0 * self->v * self->vy / UPDATES_PER_SECOND));
}

void particle_render(struct Particle *self, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &self->rect);
}
// END NAV_Particle


// NAV_ParticleManager
void particle_manager_init(struct ParticleManager *self, struct Particle *particles, int n, int w, int h, int v) {
    self->particles = particles;
    self->n = n;

    for (int i = 0; i < self->n; i++) {
        particle_init(&self->particles[i], 0, 0, w, h, v);
    }
}

struct Particle *particle_manager_get_free(struct ParticleManager *self) {
    struct Particle *particle = NULL;

    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 1) {
            continue;
        }

        particle = &self->particles[i];

        break;
    }

    return particle;
}

void particle_manager_update(struct ParticleManager *self) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_update(&self->particles[i]);
    }
}

void particle_manager_render(struct ParticleManager *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->n; i++) {
        if (self->particles[i].alive == 0) {
            continue;
        }

        particle_render(&self->particles[i], renderer);
    }
}
// END NAV_ParticleManager


// NAV_CollisionManager
void collision_manager_init(struct CollisionManager *self, struct ParticleManager *particle_manager, struct Player *player, struct Bullet *player_bullets, int bullets_n, struct Enemy *enemies, int enemies_n) {
    self->particle_manager = particle_manager;
    self->player = player;
    self->player_bullets = player_bullets;
    self->bullets_n = bullets_n;
    self->enemies = enemies;
    self->enemies_n = enemies_n;
}

void collision_manager_player_vs_enemies(struct CollisionManager *self) {
    if (self->player->alive == 0) {
        return;
    }

    for (int i = 0; i < ENEMIES_MAX; i++) {
        if (self->enemies->alive == 0) {
            continue;
        }

        if (SDL_HasIntersection(&self->player->rect, &self->enemies[i].rect) == SDL_TRUE) {
            self->enemies[i].alive = 0;
            continue;
        }
    }
}

void collision_manager_enemies_vs_player_bullets(struct CollisionManager *self) {
    for (int i = 0; i < ENEMIES_MAX; i++) {
        if (self->enemies[i].alive == 0) {
            continue;
        }

        for (int j = 0; j < PLAYER_BULLETS_MAX; j++) {
            if (self->player_bullets[j].alive == 0) {
                continue;
            }

            if (SDL_HasIntersection(&self->enemies[i].rect, &self->player_bullets[j].rect) == SDL_TRUE) {
                self->enemies[i].alive = 0;
                self->player_bullets[j].alive = 0;

                collision_manager_make_explosion(self, self->enemies[i].x, self->enemies[i].y);
                break;
            }
        }
    }
}

void collision_manager_update(struct CollisionManager *self) {
    collision_manager_player_vs_enemies(self);
    collision_manager_enemies_vs_player_bullets(self);
}

void collision_manager_make_explosion(struct CollisionManager *self, float x, float y) {
    for (int i = 0; i < EXPLOSION_PARTICLES_N; i++) {
        struct Particle *particle = particle_manager_get_free(self->particle_manager);

        particle_set_x(particle, x);
        particle_set_y(particle, y);

        particle->vx = EXPLOSION_PARTICLES_VX[i];
        particle->vy = EXPLOSION_PARTICLES_VY[i];

        particle->alive = 1;
    }
}
// END NAV_CollisionManager


// NAV_MiscFunctions
double performance_counters_to_ms(Uint64 start, Uint64 end) {
    double ms = (double)((end - start) * 1000) / SDL_GetPerformanceFrequency();

    return ms;
}

void rand_init(uint32_t seed) {
    tinymt32_init(&TINYMT_STATE, seed);
}

int rand_n(int n) {
    uint32_t rn = tinymt32_generate_uint32(&TINYMT_STATE);
    int ret = (int)(rn >> 5) % n;

    return ret;
}
// END NAV_MiscFunctions


// NAV_InGameState
void in_game_state_init(
    struct InGameState *self,
    struct Keys *keys,
    struct Player *player,
    struct Bullet *player_bullets,
    struct BulletManager *player_bullet_manager,
    struct Enemy *enemies,
    struct EnemyManager *enemy_manager,
    struct Particle *particles,
    struct ParticleManager *particle_manager,
    struct CollisionManager *collision_manager
) {
    self->keys = keys;
    self->player = player;
    self->player_bullets = player_bullets;
    self->player_bullet_manager = player_bullet_manager;
    self->enemies = enemies;
    self->enemy_manager = enemy_manager;
    self->particles = particles;
    self->particle_manager = particle_manager;
    self->collision_manager = collision_manager;
}

void in_game_state_update(struct InGameState *self) {
    player_update(self->player);
    bullet_manager_update(self->player_bullet_manager);
    enemy_manager_update(self->enemy_manager);
    particle_manager_update(self->particle_manager);
    collision_manager_update(self->collision_manager);
}

void in_game_state_render(struct InGameState *self, SDL_Renderer *renderer) {
    particle_manager_render(self->particle_manager, renderer);
    bullet_manager_render(self->player_bullet_manager, renderer);
    enemy_manager_render(self->enemy_manager, renderer);
    player_render(self->player, renderer);
}
// END NAV_InGameState


// NAV_PauseState
void pause_state_update(struct PauseState *pause_state) {

}

void pause_state_render(struct PauseState *pause_state, SDL_Renderer *renderer) {

}
// END NAV_PauseState


// NAV_Game
void game_init(struct Game *self, struct InGameState *in_game_state) {
    self->in_game_state = in_game_state;

    self->state = STATE_IN_GAME;
}

void game_set_input_component(struct Game *self, struct InputComponent *input_component) {
    self->input_component = input_component;
}

void game_run(struct Game *self, SDL_Renderer *renderer) {
    int keep_running = 1;

    Uint64 previous = SDL_GetPerformanceCounter();
    double lag = 0.0;

    while (keep_running) {
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = performance_counters_to_ms(previous, current);

        //SDL_Log("INFO: Frame time: %f ms.", elapsed);

        previous = current;
        lag += elapsed;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                keep_running = 0;
            }

            input_component_update(self->input_component, &event);
        }

        while (lag >= MS_PER_UPDATE) {
            if (self->state == STATE_IN_GAME) {
                in_game_state_update(self->in_game_state);
            } else if (self->state == STATE_PAUSE) {
                //TODO: complete
            }

            lag -= MS_PER_UPDATE;
        }

        // RENDER

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // grey
        SDL_RenderClear(renderer);

        if (self->state == STATE_IN_GAME) {
            in_game_state_render(self->in_game_state, renderer);
        } else if (self->state == STATE_PAUSE) {
            //TODO: complete
        }

        text_render(&TEXT, renderer, "UVWXYZ", 354, 8);
        SDL_RenderPresent(renderer);

        //SDL_Delay(SLEEP_MS);
    }
}
// END NAV_Game


// NAV_InputComponent
void input_component_init(
    struct InputComponent *self,
    struct Keys *keys,
    struct Game *game,
    struct Player *player
) {
    self->keys = keys;
    self->game = game;
    self->player = player;
}

void input_component_update(struct InputComponent *self, SDL_Event *event) {
    int sym;

    if (event->type == SDL_KEYDOWN) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right = 1;
        } else if (sym == SDLK_LEFT) {
            self->keys->left = 1;
        } else if (sym == SDLK_UP) {
            self->keys->up = 1;
        } else if (sym == SDLK_DOWN) {
            self->keys->down = 1;
        } else if (sym == SDLK_z) {
            self->keys->z = 1;

            player_on_button_a_keydown(self->player);
        }
    } else if (event->type == SDL_KEYUP) {
        if (event->key.repeat) {
            return;
        }

        sym = event->key.keysym.sym;

        if (sym == SDLK_RIGHT) {
            self->keys->right = 0;
        } else if (sym == SDLK_LEFT) {
            self->keys->left = 0;
        } else if (sym == SDLK_UP) {
            self->keys->up = 0;
        } else if (sym == SDLK_DOWN) {
            self->keys->down = 0;
        } else if (sym == SDLK_z) {
            self->keys->z = 0;

            player_on_button_a_keyup(self->player);
        } else if (sym == SDLK_ESCAPE) {
            if (self->game->state == STATE_IN_GAME) {
                self->game->state = STATE_PAUSE;
            } else if (self->game->state == STATE_PAUSE) {
                self->game->state = STATE_IN_GAME;
            }
        }
    }
}
// END NAV_InputComponent


// NAV_Main
int main(void) {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("ERROR: SDL_Init() (%s)", SDL_GetError());
        return 1;
    }

    // create window
    SDL_Window *window = SDL_CreateWindow("WAT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, 0);

    if (window == NULL) {
        SDL_Log("ERROR: SDL_CreateWindow() (%s)", SDL_GetError());

        SDL_Quit();

        return 1;
    }

    // create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_Log("ERROR: SDL_CreateRenderer() (%s)", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    rand_init(time(NULL));
    text_init(&TEXT);
    bullet_manager_init(&PLAYER_BULLET_MANAGER, PLAYER_BULLETS, PLAYER_BULLETS_MAX, PLAYER_BULLETS_W, PLAYER_BULLETS_H, PLAYER_BULLETS_V);
    player_init(&PLAYER, &KEYS, &PLAYER_BULLET_MANAGER, WINDOW_W / 2, WINDOW_H / 2, PLAYER_W, PLAYER_H, PLAYER_V);
    enemy_manager_init(&ENEMY_MANAGER, ENEMIES, ENEMIES_MAX, ENEMY_W, ENEMY_H, ENEMY_V);
    particle_manager_init(&PARTICLE_MANAGER, PARTICLES, PARTICLES_MAX, PARTICLE_W, PARTICLE_H, PARTICLE_V);
    collision_manager_init(&COLLISION_MANAGER, &PARTICLE_MANAGER, &PLAYER, PLAYER_BULLETS, PLAYER_BULLETS_MAX, ENEMIES, ENEMIES_MAX);

    in_game_state_init(
        &IN_GAME_STATE,
        &KEYS,
        &PLAYER,
        PLAYER_BULLETS,
        &PLAYER_BULLET_MANAGER,
        ENEMIES,
        &ENEMY_MANAGER,
        PARTICLES,
        &PARTICLE_MANAGER,
        &COLLISION_MANAGER
    );

    game_init(&GAME, &IN_GAME_STATE);

    input_component_init(&INPUT_COMPONENT, &KEYS, &GAME, &PLAYER);

    game_set_input_component(&GAME, &INPUT_COMPONENT);

    game_run(&GAME, renderer);

    // destroy renderer
    SDL_DestroyRenderer(renderer);

    // destroy window
    SDL_DestroyWindow(window);

    // quit SDL
    SDL_Quit();

    return 0;
}
// END NAV_Main

// END NAV_Code
