/* 输出 riscv */
OUTPUT_ARCH( "riscv" )

/* 代码入口 */
ENTRY( _start )

/* 定义内存，从 0x80000000 开始，长度为 128M，权限：可写、可执行，非只读，非初始化 */
MEMORY
{
	ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* 内存段 */
SECTIONS
{
    /* 代码段 */
	.text : {
		PROVIDE(_text_start = .);     /* 代码段开始变量 _text_start */
		*(.text .text.*)              /* 正则匹配 .text 开始的段都是代码段 */
		PROVIDE(_text_end = .);       /* 代码段结束变量 _text_end */
	} >ram     /* 置入 ram 中 */

    /* 只读数据段 */
	.rodata : {
		PROVIDE(_rodata_start = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	} >ram

    /* 数据段 */
	.data : {
		. = ALIGN(4096);             /* 16字节对齐 */
		PROVIDE(_data_start = .);
		*(.sdata .sdata.*)
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} >ram

    /* 未初始化数据段 */
	.bss :{
		PROVIDE(_bss_start = .);  /* bss段开始变量 _bss_start */
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		*(COMMON)
		PROVIDE(_bss_end = .);   /* bss段开始变量 _bss_end */
	} >ram

    /* 内存开始 */
	PROVIDE(_memory_start = ORIGIN(ram));
    /* 内存结束 */
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

    /* 堆内存就是 ram 剩下的内存，可供程序使用 */
    /* 堆内存开始 */
	PROVIDE(_heap_start = _bss_end);
    /* 堆内存大小 */
	PROVIDE(_heap_size = _memory_end - _heap_start);
}
