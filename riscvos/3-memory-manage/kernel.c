#include "os.h"

extern void uart_init(void);
extern void page_init(void);
extern void page_test(void);

void start_kernel(void)
{
    // init uart for system out
    uart_init();
    // puts hello world to console
    uart_puts("Hello, World\n");

    // init memory page memagement
    page_init();
    page_test();

    // loop for nothing
    while (1)
    {
    }
}