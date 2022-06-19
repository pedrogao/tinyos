#include "syscall.h"

// 系统调用 get hart id
.global gethid
gethid:
    li a7, SYS_gethid
    ecall # 陷入内核态
    ret