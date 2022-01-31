#include "platform.h"

    .equ    STACK_SIZE, 1024    # 栈大小为1024字节
    .global _start              # 全局声明 _start
    .text                       # 代码段

_start:
    csrr t0, mhartid            # 读取hart id，可以理解为 cpu id
    mv tp, t0                   # 将 hart id 保存在 tp 中
    bnez t0, park               # 比较 t0 是否为 0，如果不是，则跳到 park 处执行

    # 将 bss 段全部数据设置为0，清除debug信息
    la a0, _bss_start
    la a1, _bss_end
    bgeu a0, a1, 2f            # 如果 a0==a1，则跳到2，否则进入1
1:
    sw zero, (a0)              # 设置a0=0
    addi a0, a0, 4             # a0++
    bltu a0, a1, 1b            # 比较 a0!=a1，跳转到1
2:
    # 如果 hart id 是 0，则按照下面的代码执行
    slli t0, t0, 10             # sheft left，把 t0 左移10位，得到当前hart栈所在的位置偏移
    la sp, stacks + STACK_SIZE  # 设置 sp 寄存器为栈底，高位内存，栈从上往下
    add sp, sp, t0              # sp = sp + t0，将 sp 移动到当前 hart 栈的栈底，所以 tp 保存至关重要
    j  start_kernel             # 注意：只有 hart0 跳转

park:
    wfi                         # 啥也不干，wait for interupt
    j park                      # 死循环
    
stacks:
    .skip STACK_SIZE * MAXNUM_CPU # 每个CPU都有一个栈，每个栈都是1024字节

    .end                          # end of file，eof