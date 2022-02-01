#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10    // 最多10个任务
#define STACK_SIZE 1024 // 栈大小

uint8_t task_stack[MAX_TASKS][STACK_SIZE]; // 10*1024字节
struct context ctx_tasks[MAX_TASKS];       // 所有进程上下文

static int _top = 0;
static int _current = -1;

void sched_init()
{
    w_mscratch(0); // 清零 mscratch
}

void schedule()
{
    if (_top <= 0)
    {
        panic("Num of task should be greater than zero!");
        return;
    }

    _current = (_current + 1) % _top;
    struct context *next = &(ctx_tasks[_current]); // 调度到下一个进程，即 user_task0 进程
    switch_to(next);
}

/*
 * task_create:
 *  1. set sp,ra
 *  2. top++
 */
int task_create(void (*start_func)(void))
{
    if (_top < MAX_TASKS)
    {
        ctx_tasks[_top].sp = (reg_t)&task_stack[_top][STACK_SIZE - 1];
        ctx_tasks[_top].ra = (reg_t)start_func;
        _top++;
        return 0;
    }
    return -1;
}

/*
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
    schedule(); // call schedule
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