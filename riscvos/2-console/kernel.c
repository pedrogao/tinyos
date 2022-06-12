extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel(void)
{
    // init uart for system out
    uart_init();
    // puts hello world to console
    uart_puts("Hello, World\n");
    // loop for nothing
    while (1)
    {
    }
}