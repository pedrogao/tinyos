#include "platform.h"

# 定义栈大小
.equ STACK_SIZE, 1024

.global _start

.text

_start:
    csrr t0, mhartid # read machine hart id
    mv tp, t0        # tp = t0
    bnez t0, park    # if t0 != 0, goto park
                     # 每一个 hart 都有一个栈，栈大小为 1024
    slli t0, t0, 10  # t0 = t0 << 10, 1 << 10 = 1024
    la sp, stacks + STACK_SIZE 
                     # 移动到 hart 对应的 sp，栈顶
    add sp, sp, t0   # sp = sp + 1024 * n

    j start_kernel   # hart 0 goto kernel

park:
                     # wait for interrupt，无任何有用工作，摆烂就行
    wfi
    j park

                     # 栈，比如 8 个 hart，那就是 8 * 1024

                     
stacks:
    .skip STACK_SIZE * MAXNUM_CPU

.end
