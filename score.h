#ifndef SCORE_H
#define SCORE_H

struct Score {
    unsigned long value;
};

void score_init(
    struct Score *self
);

#endif
