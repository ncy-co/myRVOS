/* 
 * Author: bye
 * Date: 2023-09-19 18:33:05
 * LastEditors: bye
 * LastEditTime: 2023-09-22 19:26:32
 * FilePath: /study/RVOS/code/myRVOS/07_hardwareTimer/riscv.h
 * Description: 寄存器的设置函数
 */
#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

// read tp
static inline reg_t _r_tp(){
    reg_t x;
    asm volatile (
        "mv %0, tp"
        :"=r"(x)
        :
    );
    return x;
}

// read mhartid
static inline reg_t _r_mhartid(){
    reg_t x;
    asm volatile (
        "csrr %0, mhartid"
        :"=r"(x)
        :
    );
    return x;
}

// 想得到 MSTATUS中的MPP 的值：mstatus与MSTATUS_MPP 按位与，之后在进行移位处理
#define MSTATUS_MPP (3 << 11)
#define MSTATUS_SPP (1 << 8)

#define MSTATUS_MPIE (1 << 7)
#define MSTATUS_SPIE (1 << 5)
#define MSTATUS_UPIE (1 << 4)

#define MSTATUS_MIE (1 << 3)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_UIE (1)

// read mstatus
static inline reg_t _r_mstatus() {
    reg_t x;
    asm volatile (
        "csrr %0, mstatus"
        :"=r"(x)
        :
    );
    return x;
}

// write mstatus
static inline void _w_mstatus(reg_t x) {
    asm volatile (
        "csrw mstatus, %0"
        :
        :"r"(x)
    );
}

// write mepc
static inline void _w_mepc(reg_t x) {
    asm volatile (
        "csrw mepc, %0"
        :
        :"r"(x)
    );
}

// read mepc
static inline reg_t _r_mepc() {
    reg_t x;
    asm volatile (
        "csrr %0, mepc"
        :"=r"(x)
        :
    );
    return x;
}

// write mscratch
static inline void _w_mscratch(reg_t x) {
    asm volatile (
        "csrw mscratch, %0"
        :
        :"r"(x)
    );
}

// write mtvec
static inline void _w_mtvec(reg_t x) {
    asm volatile (
        "csrw mtvec, %0"
        :
        :"r"(x)
    );
}

#define MIE_MEIE (1 << 11)
#define MIE_MTIE (1 << 7)
#define MIE_MSIE (1 << 3)

// read mie
static inline reg_t _r_mie() {
    reg_t x;
    asm volatile (
        "csrr %0, mie"
        :"=r"(x)
        :
    );
    return x;
}

// write mie
static inline void _w_mie(reg_t x) {
    asm volatile (
        "csrw mie, %0"
        :
        :"r"(x)
    );
}

// read mcause
static inline reg_t _r_mcause() {
    reg_t x;
    asm volatile (
        "csrr %0, mcause"
        :"=r"(x)
        :
    );
    return x;
}

#endif
