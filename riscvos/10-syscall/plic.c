#include "os.h"

// interrupt controller 中断处理器

void plic_init(void)
{
    // current hart id
    int hart = r_tp();
    // 设置 uart0 优先级
    *(uint32_t *)PLIC_PRIORITY(UART0_IRQ) = 1;
    // 开启 uart0
    *(uint32_t *)PLIC_MENABLE(hart) = (1 << UART0_IRQ);
    // 设置 uart0 优先级阈值
    *(uint32_t *)PLIC_MTHRESHOLD(hart) = 0;

    /* enable machine-mode external interrupts. */
    w_mie(r_mie() | MIE_MEIE);

    /* enable machine-mode global interrupts. */
    w_mstatus(r_mstatus() | MSTATUS_MIE);
}

int plic_claim(void)
{
    int hart = r_tp();
    int irq = *(uint32_t *)PLIC_MCLAIM(hart);
    return irq;
}

void plic_complete(int irq)
{
    int hart = r_tp();
    *(uint32_t *)PLIC_MCOMPLETE(hart) = irq;
}
