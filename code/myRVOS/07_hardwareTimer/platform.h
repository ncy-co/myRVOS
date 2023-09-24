/* 
 * Author: bye
 * Date: 2023-09-20 16:57:51
 * LastEditors: bye
 * LastEditTime: 2023-09-22 19:27:27
 * FilePath: /study/RVOS/code/myRVOS/07_hardwareTimer/platform.h
 * Description: 
 */
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//  Define information about the platform (development board)


// number of hart is 8
#define MAXNUM_CPU 8

// define the start address of the UART0 
#define UART0 0x10000000L

// uart的中断源编号为10
#define UART0_IRQ 10

// platform-level interrupt controller
#define PLIC_BASE 0x0c000000L
// read address of priority
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)
// 获取 PENDING 寄存器的地址
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4)
// 获取 MENABLE 寄存器的地址
#define PLIC_MENABLE(hart) (PLIC_BASE + 0x2000 + (hart) * 0x80)
// 获取 MTHRESHOLD 寄存器的地址
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)
// 获取 MCLAIM 寄存器的地址
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)
// 获取 MCOMPLETE 寄存器的地址
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)

// 时钟相关的寄存器地址
#define CLINT_BASE 0x2000000L
// 获取CLINT_MSIP的地址
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid))
// 获取 CLINT_MTIMECMP 的地址
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
// 获取 CLINT_MTIME 的地址
#define CLINT_MTIME (CLINT_BASE + 0xBFF8)

// 10000000 ticks per-second
#define CLINT_TIMEBASE_FREQ 10000000


#endif