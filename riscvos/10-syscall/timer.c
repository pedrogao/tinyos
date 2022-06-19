#include "os.h"

extern void schedule(void);

// 1s
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;

void timer_load(int interval)
{
    int id = r_mhartid();

    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init()
{
    timer_load(TIMER_INTERVAL);

    /* enable machine-mode timer interrupts. */
    w_mie(r_mie() | MIE_MTIE);
}

void timer_handler()
{
    _tick++;
    printf("tick: %d\n", _tick);
    timer_load(TIMER_INTERVAL);

    schedule();
}