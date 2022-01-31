include ../common.mk

# 汇编源文件
SRCS_ASM = \
	start.S \

# c 源文件
SRCS_C = \
	kernel.c \

# 目标文件
OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

# 默认目标
.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
# os.elf 依赖于 OBJS
os.elf: ${OBJS}
	${CC} ${CFLAGS} -Ttext=0x80000000 -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

# 编译c文件
%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

# 编译汇编文件
%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

# 运行qemu
run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

# debug 运行，运行qemu和gdb
.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ../gdbinit

# 查询 os.elf
.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

# clean
.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf