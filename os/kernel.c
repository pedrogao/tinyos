#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 * 调用一次，其它类库不可调用
 */
extern void uart_init(void);
extern void page_init(void);

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello tinyos!\n");

    page_init();

    while (1)
    {
    }; // 死循环，进入内核后啥也不干
}