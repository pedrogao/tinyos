#include "os.h"

#define DELAY 1000

#define USE_LOCK

/* 0号进程任务 */
void user_task0(void)
{
    uart_puts("Task 0: Created!\n");
    while (1)
    {
#ifdef USE_LOCK
        spin_lock();
#endif
        uart_puts("Task 0: Begin...\n");
        for (int i = 0; i < 5; i++)
        {
            uart_puts("Task 0: Running...\n");
            task_delay(DELAY);
        }
        uart_puts("Task 0: End...\n");
#ifdef USE_LOCK
        spin_unlock();
#endif
    }
}

/* 1号进程任务 */
void user_task1(void)
{
    // 抢占式
    uart_puts("Task 1: Created!\n");
    while (1)
    {
        uart_puts("Task 1: Running...\n");
        task_delay(DELAY);
    }
}

void os_main()
{
    task_create(user_task0);
    task_create(user_task1);
}