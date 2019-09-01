#ifndef UTIL_H
#define UTIL_H

#include "xoshiro128.h"
#include "SDL.h"

double performance_counters_to_ms(Uint64 start, Uint64 end);

void rand_init(uint32_t *state, long initial_seed);
int rand_n(uint32_t *state, int n);

#endif
