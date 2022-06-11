# load byte(signed)
# format:
# lb RD, IMM(RS1)
# 从内存中加载 8 位数据，即 1 字节

.text
.global _start

_start:
    la x5, _array # x5 = array
    lb x6, 0(x5)  # x6 = array[0]
    lb x7, 1(x5)  # x7 = array[1]
stop:
    j stop        # jump to stop

_array:           # define _array
    .byte 0x11
    .byte 0xff

.end