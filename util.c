#include "util.h"

double performance_counters_to_ms(Uint64 start, Uint64 end) {
    double ms = (double)((end - start) * 1000) / SDL_GetPerformanceFrequency();

    return ms;
}

void rand_init(uint32_t *state, long initial_seed) {
    state[0] = initial_seed;
    state[1] = ++state[0];
    state[2] = ++state[1];
    state[3] = ++state[2];
}

int rand_n(uint32_t *state, int n) {
    jump(state);
    int ret = (int)(state[1] >> 5) % n;

    return ret;
}
