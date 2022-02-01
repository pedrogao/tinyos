# save all General-Purpose(GP) registers to context
# struct context *base = &ctx_task;
# base->ra = ra;
# ......
.macro reg_save base
	sw ra, 0(\base)
	sw sp, 4(\base)
	sw gp, 8(\base)
	sw tp, 12(\base)
	sw t0, 16(\base)
	sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
	# we don't save t6 here, due to we have used
	# it as base, we have to save t6 in an extra step
	# outside of reg_save
.endm

# restore all General-Purpose(GP) registers from the context
# struct context *base = &ctx_task;
# ra = base->ra;
# ......
.macro reg_restore base
	lw ra, 0(\base)
	lw sp, 4(\base)
	lw gp, 8(\base)
	lw tp, 12(\base)
	lw t0, 16(\base)
	lw t1, 20(\base)
	lw t2, 24(\base)
	lw s0, 28(\base)
	lw s1, 32(\base)
	lw a0, 36(\base)
	lw a1, 40(\base)
	lw a2, 44(\base)
	lw a3, 48(\base)
	lw a4, 52(\base)
	lw a5, 56(\base)
	lw a6, 60(\base)
	lw a7, 64(\base)
	lw s2, 68(\base)
	lw s3, 72(\base)
	lw s4, 76(\base)
	lw s5, 80(\base)
	lw s6, 84(\base)
	lw s7, 88(\base)
	lw s8, 92(\base)
	lw s9, 96(\base)
	lw s10, 100(\base)
	lw s11, 104(\base)
	lw t3, 108(\base)
	lw t4, 112(\base)
	lw t5, 116(\base)
	lw t6, 120(\base)
.endm

# Something to note about save/restore:
# - We use mscratch to hold a pointer to context of previous task
# - We use t6 as the 'base' for reg_save/reg_restore, because it is the
#   very bottom register (x31) and would not be overwritten during loading.

.text

# void switch_to(struct context *next);
# a0: pointer to the context of the next task
.globl switch_to
.align 4
switch_to:
    csrrw t6, mscratch, t6   # swap t6, mscratch，将 t6 中的数据暂时保存到 mscratch
    beqz  t6, 1f             # if t6 == 0, jump 1f, 上一个 context 是 NULL
    reg_save t6              # t6 保存了context地址

    mv t5, t6                # t5 = t6
    csrr t6, mscratch        # t6 = mscratch
    sw t6, 120(t5)           # t6 = t5 + 120，t6 指向了下一个 context 首地址

1:
    csrw mscratch, a0        # mscratch = a0，将a0暂时保存到mscratch中 
    mv t6, a0                # t6 = a0, a0 是第一个参数的地址，即 next context
    reg_restore t6           # context 恢复，context 首地址在 t6 中

    ret                      # return

.end                         # end of file