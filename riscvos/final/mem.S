.section .rodata              # 只读段
.global HEAP_START
HEAP_START: .word _heap_start # ld脚本提供，堆开始地址

.global HEAP_SIZE
HEAP_SIZE: .word _heap_size   # 堆大小

.global TEXT_START
TEXT_START: .word _text_start # 代码段开始地址

.global TEXT_END
TEXT_END: .word _text_end     # 代码段结束地址

.global DATA_START
DATA_START: .word _data_start # 数据段开始地址

.global DATA_END
DATA_END: .word _data_end     # 数据段结束地址

.global RODATA_START
RODATA_START: .word _rodata_start # 只读段开始地址

.global RODATA_END
RODATA_END: .word _rodata_end     # 只读段结束地址

.global BSS_START
BSS_START: .word _bss_start       # bss 开始地址

.global BSS_END
BSS_END: .word _bss_end           # bss 结束地址