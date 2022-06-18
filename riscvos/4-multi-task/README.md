# 任务与多任务

```s
switch_to:
    # a0: pointer to the context of the next task
    # mscratch 在 sched_init 时为 0
    csrrw t6, mscratch, t6 # swap t6, mscratch
    beqz t6, do_switch     # if t6 == 0, goto do_switch
    reg_save t6            # save prev context

    # 由于 reg_save 时没有保存t6，因此此处将t6存储到120偏移处
    mv t5, t6              # t6 = t5
    csrr t6, mscratch      # t6 = mscratch
    sw t6, 120(t5)
do_switch:
    csrw mscratch, a0      # mscratch = a0
    mv t6, a0
    reg_restore t6

    ret
```

通过判断 `mscratch` 是否为 0，来决定是否需要 `reg_save`。
