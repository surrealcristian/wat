#ifndef UTIL_H
#define UTIL_H

#include "tinymt32.h"
#include "SDL.h"

double performance_counters_to_ms(
    Uint64 start,
    Uint64 end
);

void rand_init(
    tinymt32_t *state,
    uint32_t   seed
);

int rand_n(
    tinymt32_t *state,
    int        n
);

#endif
