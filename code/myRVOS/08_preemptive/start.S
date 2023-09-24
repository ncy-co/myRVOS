#include "platform.h"

    # size of each hart's stack is 1024 bytes
    .equ STACK_SIZE, 1024
    .global _start
    .text

_start:
    # park harts with hartid != 0
    csrr t0, mhartid
    mv tp, t0
    bnez t0, park

    la a0, _bss_start
    la a1, _bss_end
    // if _bss is not empty
    bgeu a0, a1, 2f

1:
    // clear _bss
    sw zero, (a0)
    addi a0, a0, 4
    // 判断是否已经清空
    bltu a0, a1, 1b

2:
    # init stack (系统栈）
    slli t0, t0, 10
    la sp, stacks + STACK_SIZE
    add sp, sp, t0

    li t0, 3 << 11 | 1 << 7
    csrr a1, mstatus
    or t0, t0, a1
    csrw mstatus, t0

    j start_kernel


park:
    # sleep hart
    wfi
    j park

    # In the standard RISC-V calling convention, the stack pointer sp is always 16-byte aligned.
.align 16
stacks:
    .skip MAXNUM_CPU * STACK_SIZE
    
    .end
