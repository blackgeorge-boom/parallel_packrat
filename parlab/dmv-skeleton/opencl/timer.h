#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

#define USEC_PER_SEC    1000000L

struct xtimer {
    struct timeval elapsed_time;
    struct timeval timestamp;
};

typedef struct xtimer   xtimer_t;

void timer_clear(xtimer_t *timer);
void timer_start(xtimer_t *timer);
void timer_stop(xtimer_t *timer);
double timer_elapsed_time(xtimer_t *timer);

#endif  /* TIMER_H */
