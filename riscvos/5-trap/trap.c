#include "os.h"

extern void trap_vector(void);

void trap_init()
{
    // 设置 trap 处理函数
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
    reg_t return_pc = epc;
    reg_t cause_code = cause & 0xfff;
    if (cause & 0x80000000)
    {
        // 异步 trap，interrupt，中断
        switch (cause_code)
        {
        case 3:
            uart_puts("software interruption!\n"); // 软件中断
            break;
        case 7:
            uart_puts("timer interruption!\n"); // 时钟中断
            break;
        case 11:
            uart_puts("external interruption!\n"); // 外设中断
            break;
        default:
            uart_puts("unknown async exception!\n");
            break;
        }
    }
    else
    {
        // 同步 trap，exception，异常
        printf("sync exceptions!, code = %d\n", cause_code);
        panic("can't go here!");
    }

    // 返回原始pc
    return return_pc;
}

void trap_test()
{
    // 访问非法内存，会进入异常
    *(int *)0x00000000 = 100;

    uart_puts("Yeah! I'm return back from trap!\n");
}