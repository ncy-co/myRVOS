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

    # init stack
    slli t0, t0, 10
    la sp, stacks + STACK_SIZE
    add sp, sp, t0

    j start_kernel

park:
    wfi
    j park

    # In the standard RISC-V calling convention, the stack pointer sp is always 16-byte aligned.
.align 16
stacks:
    .skip MAXNUM_CPU * STACK_SIZE
    
    .end
