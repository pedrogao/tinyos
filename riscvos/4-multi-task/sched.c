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

static void w_mscratch(reg_t x)
{
    // swap mscratch, x
    asm volatile("csrw mscratch, %0"
                 :
                 : "r"(x));
}

void sched_init()
{
    // mscratch = 0
    w_mscratch(0);
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
        // return address
        // 调用 ret 时，ra 的值会赋给 ip 寄存器，然后从 ip 开始运行
        ctx_tasks[_top].ra = (reg_t)start_routine;
        _top++;
        return 0;
    }
    return -1;
}

void task_yield()
{
    schedule();
}

void task_delay(volatile int count)
{
    count *= 50000;
    // 空转
    while (count--)
        ;
}