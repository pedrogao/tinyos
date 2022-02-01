#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define STACK_SIZE 1024 // 栈大小

uint8_t task_stack[STACK_SIZE]; // 1024字节
struct context ctx_task;        // 当前进程上下文

/* write mscratch */
static void w_mscratch(reg_t x)
{
    asm volatile("csrw mscratch, %0" ::"r"(x));
}

/* 用户态第一个进程 */
void user_task0(void);

void sched_init()
{
    w_mscratch(0); // 清零 mscratch

    ctx_task.sp = (reg_t)&task_stack[STACK_SIZE - 1]; // 栈底，高位地址
    ctx_task.ra = (reg_t)user_task0;                  // 返回地址是 user_task0
}

void schedule()
{
    struct context *next = &ctx_task; // 调度到下一个进程，即 user_task0 进程
    switch_to(next);
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
    count *= 50000;
    while (count--)
        ;
}

void user_task0(void)
{
    uart_puts("Task 0: Created!\n");
    while (1) // 死循环，打印日志
    {
        uart_puts("Task 0: Running...\n");
        task_delay(1000);
    }
}