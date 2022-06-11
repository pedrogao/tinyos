# RISC-V 汇编入门

## 环境搭建

在 Linux 下，直接下载 `riscv` 和 `qemu-riscv` 即可。 如果是 MAC，直接使用`brew`来安装：

```sh
brew install qemu-riscv
brew install riscv64-unknown-elf-gcc
```

TODO

## 数据加载、存储

### 加载立即数

```s
# Load Immediate
li rd, imm # rd = imm，加载立即数

# Load Upper Immediate
lui rd, imm # rd = imm << 12，加载立即数，upper
```

### 加载内存

```s
# Load Byte (Signed)
lb rd, imm(rs1) # rd = [rs1 + imm]

# Load Byte (Unsigned)
lbu rd, imm(rs1) # unsigned rd = [rs1 + imm]
```

### 加载地址

```s
# Load Address
la x5, label # x5 = label
```

### 存储内存

```s
# Store Byte
sb rs2, imm(rs1) # [rs1 + imm] = rs2
```

### 寄存器数据传输

```s
# Move (Register to Register)
mv x5, x6		# x5 = x6
```

## 运算符操作

### 加、减法

```s
# Add
add x5, x6, x7		# x5 = x6 + x7

# Add Immediate
addi x5, x6, 1		# x5 = x6 + 1

# Substract
sub x5, x6, x7		# x5 = x6 - x7

# Substract Immediate
addi x5, x6, -20	# x5 = x6 - 20
```

### 左、右移

```s
# Shift Left Logical Immediate
slli x5, x6, 3		# x5 = x6 << 3

# Shift Right Arithmetic Immediate 算术右移，带符号
srai x5, x6, 4		# x5 = x6 >> 4

# Shift Right Logical Immediate 逻辑右移，补0
srli x5, x6, 3		# x5 = x6 >> 3
```

### 逻辑运算

```s
# Negate
neg x5, x6		# x5 = -x6

# Not
not x5, x6		# x5 = ~x6

# xori 异或
xori x5, x6, -1	 # x5 = x6 ^ -1
```

### nop

```s
# Nop
nop			# do nothing and has no effect on system
```

## 分支跳转

### 直接跳转

```s
# Add Upper Immediate to PC
auipc x5, 0x12345	# x5 = PC + (0x12345 << 12)
auipc x6, 0		    # x6 = PC, to obtain the current PC
```

### 比较跳转

```s
# Branch if Not Equal
bne x5, x6, loop    # if x5 != x6, goto loop
```

## 函数调用

### square 计算平方

```s
# void _start()
# {
#     // calling leaf routine
#     square(3);
# }
#
# int square(int num)
# {
#     return num * num;
# }

.text			# Define beginning of text section
.global	_start		# Define entry _start

_start:
	la sp, stack_end	# prepare stack for calling functions

	li a0, 3
	call square

	# the time return here, a0 should stores the result
stop:
	j stop			# Infinite loop to stop execution

# int square(int num)
square:
	# prologue
	addi sp, sp, -8
	sw s0, 0(sp)
	sw s1, 4(sp)

	# `mul a0, a0, a0` should be fine,
	# programing as below just to demo we can contine use the stack
	mv s0, a0
	mul s1, s0, s0
	mv a0, s1

	# epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	addi sp, sp, 8

	ret
	# add nop here just for demo in gdb
	nop

	# allocate stack space
stack_start:
	.rept 12
	.word 0
	.endr
stack_end:

.end			# End of file
```

函数执行，需要先开辟函数调用栈，即`stack_start`，且内存高位设置为`sp`，指向
栈顶。

`s0`，`s1`是传递参数，返回值为`ra`寄存器。

### 多级调用 square

```s
# void _start()
# {
#     // calling nested routine
#     aa_bb(3, 4);
# }
#
# int aa_bb(int a, int b)
# {
#     return square(a) + square(b);
# }
#
# int square(int num)
# {
#     return num * num;
# }

.text			# Define beginning of text section
.global	_start		# Define entry _start

_start:
	la sp, stack_end	# prepare stack for calling functions

	# aa_bb(3, 4);
	li a0, 3
	li a1, 4
	call aa_bb

stop:
	j stop			# Infinite loop to stop execution

# int aa_bb(int a, int b)
# return a^2 + b^2
aa_bb:
	# prologue
	addi sp, sp, -16
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw ra, 12(sp)

	# cp and store the input params
	mv s0, a0
	mv s1, a1

	# sum will be stored in s2 and is initialized as zero
	li s2, 0

	mv a0, s0
	jal square
	add s2, s2, a0

	mv a0, s1
	jal square
	add s2, s2, a0

	mv a0, s2

	# epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	ret

# int square(int num)
square:
	# prologue
	addi sp, sp, -8
	sw s0, 0(sp)
	sw s1, 4(sp)

	# `mul a0, a0, a0` should be fine,
	# programing as below just to demo we can contine use the stack
	mv s0, a0
	mul s1, s0, s0
	mv a0, s1

	# epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	addi sp, sp, 8

	ret

	# add nop here just for demo in gdb
	nop

	# allocate stack space
stack_start:
	.rept 12
	.word 0
	.endr
stack_end:

.end			# End of file
```

先调用`aa_bb`，然后再调用两次`square`，参数不同，然后将结果相加返回。

## 与 C 交互

先定义 C 函数`foo`：

```c
int foo(int a, int b)
{
	int c = a + b;
	return c;
}
```

然后在`test.s`中调用：

```s
# ASM call C

.text			# Define beginning of text section
.global	_start		# Define entry _start
.global	foo		# foo is a C function defined in test.c

_start:
	la sp, stack_end	# prepare stack for calling functions

	# RISC-V uses a0 ~ a7 to transfer parameters
	li a0, 1
	li a1, 2
	call foo
	# RISC-V uses a0 & a1 to transfer return value
	# check value of a0

stop:
	j stop			# Infinite loop to stop execution

	nop			# just for demo effect

stack_start:
	.rept 12
	.word 0
	.endr
stack_end:

.end			# End of file
```

## 运行

```sh
make run M=lb

make debug M=lb
```

```sh
$ riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall -Ttext=0x80000000 -o a.elf ./asm.S

$ riscv64-unknown-elf-objcopy -O binary a.elf a.bin

$ qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel a.elf

$ qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel a.elf -s -S &
$ riscv64-unknown-elf-gdb -q

set disassemble-next-line on
b _start
target remote : 1234
c
```

## 参考资料

- https://www.ruanyifeng.com/blog/2015/02/make.html
