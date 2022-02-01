#include "os.h"

#define DELAY 1000

/* 0号进程任务 */
void user_task0(void)
{
    uart_puts("Task 0: Created!\n");
    while (1)
    {
        uart_puts("Task 0: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

/* 1号进程任务 */
void user_task1(void)
{
    uart_puts("Task 1: Created!\n");
    while (1)
    {
        uart_puts("Task 1: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

void os_main()
{
    task_create(user_task0);
    task_create(user_task1);
}