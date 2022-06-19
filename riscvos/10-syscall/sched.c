#include "os.h"

/* refer entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024

// 栈
uint8_t task_stack[MAX_TASKS][STACK_SIZE];
// 上下文
struct context ctx_tasks[MAX_TASKS];

// stack top 栈顶
static int _top = 0;
// 当前 task
static int _current = -1;

void sched_init()
{
    // mscratch = 0
    w_mscratch(0);
    /* enable machine-mode software interrupts. */
    w_mie(r_mie() | MIE_MSIE);
}

void schedule()
{
    if (_top <= 0)
    {
        panic("Num of task should be greater than zero!");
        return;
    }

    _current = (_current + 1) % _top;
    struct context *next = &(ctx_tasks[_current]);
    switch_to(next);
}

int task_create(void (*start_routine)(void))
{
    if (_top < MAX_TASKS)
    {
        // 栈顶
        ctx_tasks[_top].sp = (reg_t)&task_stack[_top][STACK_SIZE - 1];
        ctx_tasks[_top].pc = (reg_t)start_routine;
        _top++;
        return 0;
    }
    return -1;
}

void task_yield()
{
    // 触发软中断
    int id = r_mhartid();
    *(uint32_t *)CLINT_MSIP(id) = 1;
}

void task_delay(volatile int count)
{
    count *= 50000;
    // 空转
    while (count--)
        ;
}