#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel(void)
{
    // init uart for system out
    uart_init();
    // puts hello world to console
    uart_puts("Hello, World\n");

    // init memory page memagement
    page_init();

    // init trap vector
    trap_init();

    // init plic
    plic_init();

    // init timer
    timer_init();

    // init schduler
    sched_init();

    // main
    os_main();

    // schdule
    schedule();

    uart_puts("Would not go here!\n");

    // loop for nothing
    while (1)
    {
    }
}