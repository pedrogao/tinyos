# add immediate
# addi rd, rs1, imm

.text
.global _start

_start:
    li x6, 2
    addi x5, x6, 1 # x5 = x6 + 1

stop:
    j stop

.end