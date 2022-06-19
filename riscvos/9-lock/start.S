#include "platform.h"

# 定义栈大小
.equ STACK_SIZE, 1024

.global _start

.text

_start:
    csrr t0, mhartid # read machine hart id
    mv tp, t0        # tp = t0
    bnez t0, park    # if t0 != 0, goto park

    la a0, _bss_start
    la a1, _bss_end
    bgeu a0, a1, init_stack # if a0 >= a1, goto init_stack
    
zero_bss:
    # 将整个 bss 段清0 
    sw zero, (a0)
    addi a0, a0, 4 
    bltu a0, a1, zero_bss   # if a0 < a1, goto zero_bss

init_stack: 
    # 每一个 hart 都有一个栈，栈大小为 1024
    slli t0, t0, 10  # t0 = t0 << 10, 1 << 10 = 1024
    la sp, stacks + STACK_SIZE 
                     # 移动到 hart 对应的 sp，栈顶
    add sp, sp, t0   # sp = sp + 1024 * n

	# At the end of start_kernel, schedule() will call MRET to switch
	# to the first task, so we parepare the mstatus here.
	# Notice: default mstatus is 0
	# Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
	# Set mstatus.MPIE to 1, so MRET will enable the interrupt.
    li t0, 3 << 11 | 1 << 7
    csrr a1, mstatus # read mstatus to a1
    or t0, t0, a1    # reset a1
    csrw mstatus, t0 # write mstatus

    j start_kernel   # hart 0 goto kernel

park:
                     # wait for interrupt，无任何有用工作，摆烂就行
    wfi
    j park

                     # 栈，比如 8 个 hart，那就是 8 * 1024

                     
stacks:
    .skip STACK_SIZE * MAXNUM_CPU

.end
