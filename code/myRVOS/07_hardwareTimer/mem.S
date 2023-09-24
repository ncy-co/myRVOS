/* 
 * Author: bye
 * Date: 2023-09-13 10:42:05
 * LastEditors: bye
 * LastEditTime: 2023-09-13 10:45:26
 * FilePath: /study/RVOS/code/myRVOS/memanagement/mem.S
 * Description: 若想在c文件中获取ld中的地址，用汇编定义全局变量进行赋值
 */
.section .rodata
.global HEAP_START
HEAP_START : .word _heap_start

.global HEAP_SIZE
HEAP_SIZE : .word _heap_size

.global TEXT_START
TEXT_START: .word _text_start

.global TEXT_END
TEXT_END: .word _text_end

.global DATA_START
DATA_START: .word _data_start

.global DATA_END
DATA_END: .word _data_end

.global RODATA_START
RODATA_START: .word _rodata_start

.global RODATA_END
RODATA_END: .word _rodata_end

.global BSS_START
BSS_START: .word _bss_start

.global BSS_END
BSS_END: .word _bss_end