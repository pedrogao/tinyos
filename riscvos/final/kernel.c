#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 * 调用一次，其它类库不可调用
 */
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
    uart_init(); // 输出设备初始化
    uart_puts("Hello tinyos!\n");

    page_init();  // 内存初始化
    trap_init();  // trap初始化
    plic_init();  // 外部中断初始化
    timer_init(); // 时种中断初始化
    sched_init(); // 调度初始化

    os_main(); // 创建任务

    schedule(); // 调度

    uart_puts("Would not go here!\n");

    // 死循环，进入内核后啥也不干
    while (1)
        ;
}