/* 
 * Author: bye
 * Date: 2023-09-13 09:04:24
 * LastEditors: bye
 * LastEditTime: 2023-09-13 09:50:27
 * FilePath: /study/RVOS/code/myRVOS/memanagement/os.ld
 * Description: Linked Script
 */

/*
 * https://sourceware.org/binutils/docs/ld/Miscellaneous-Commands.html
 * OUTPUT_ARCH command specifies a particular output machine architecture.
 * "riscv" is the name of the architecture for both 64-bit and 32-bit
 * RISC-V target. We will further refine this by using -march=rv32ima
 * and -mabi=ilp32 when calling gcc.
 */
OUTPUT_ARCH("riscv")

ENTRY(_start)

MEMORY {
    /* 设置一块128MB的内存区，起始位置为0x80000000 */
    ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}
SECTIONS {
    .text : {
        PROVIDE(_text_start = .);
        *(.text *text.*)
        PROVIDE(_text_end = .);
    } >ram

    .rodata : {
        PROVIDE(_rodata_start = .);
        *(.rodata .rodata.*)
        PROVIDE(_rodata_end = .);
    } >ram

    .data : {
        /*
         * 将代码段和只读数据段放在一个页中
         * 将当前地址对齐到4096字节
         */ 
        . = ALIGN(4096);
        PROVIDE(_data_start = .);
        *(.sdata .sdata.*)
        *(.data .data)
        PROVIDE(_data_end = .);
    } >ram

    .bss : {
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE(_bss_end = .);
    } >ram

    PROVIDE(_memory_start = ORIGIN(ram));
    PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

    PROVIDE(_heap_start = _bss_end);
    PROVIDE(_heap_size = _memory_end - _heap_start);
}






