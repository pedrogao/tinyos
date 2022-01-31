# 检查编译前缀
CROSS_COMPILE = riscv64-unknown-elf-
# 编译flags
CFLAGS = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall # 编译为32位
# qemu
QEMU = qemu-system-riscv32
# qemu flags
QFLAGS = -nographic -smp 1 -machine virt -bios none
# gdb
GDB = riscv64-unknown-elf-gdb
# gcc
CC = ${CROSS_COMPILE}gcc
# objcopy
OBJCOPY = ${CROSS_COMPILE}objcopy
# objdump
OBJDUMP = ${CROSS_COMPILE}objdump