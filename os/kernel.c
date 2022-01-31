extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello tinyos!\n");

    while (1)
    {
    }; // 死循环，进入内核后啥也不干
}