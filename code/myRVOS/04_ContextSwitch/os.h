/* 
 * Author: bye
 * Date: 2023-09-13 09:00:03
 * LastEditors: bye
 * LastEditTime: 2023-09-18 15:36:41
 * FilePath: /study/RVOS/code/myRVOS/04_ContextSwitch/os.h
 * Description: 
 */
#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"

#include <stdarg.h>
#include <stddef.h>

// uart
extern void uart_init(void);
extern int uart_putc(char ch);
extern void uart_puts(char *s);

// printf
extern int printf(const char *s, ...);
extern void printc(char *s);

// memory management
extern void page_init();
extern void *page_alloc(int num_pages);
extern void page_free(void *p);
extern void page_test();

// task management
typedef struct context {
    reg_t ra;
    reg_t sp;
    reg_t gp; // Global Pointer
    reg_t tp; // Thread pointer
    reg_t t0; // Temporary
    reg_t t1;
    reg_t t2;
    reg_t s0; // Saved register
    reg_t s1;
    reg_t a0; // Function argument
    reg_t a1;
    reg_t a2;
    reg_t a3;
    reg_t a4;
    reg_t a5;
    reg_t a6;
    reg_t a7;
    reg_t s2;
    reg_t s3;
    reg_t s4;
    reg_t s5;
    reg_t s6;
    reg_t s7;
    reg_t s8;
    reg_t s9;
    reg_t s10;
    reg_t s11;
    reg_t t3;
    reg_t t4;
    reg_t t5;
    reg_t t6;
} context;
extern void sched_init();
extern int task_create(void (*address_task)(void));
extern void task_yield(void);
extern void task_delay(volatile int count);

#endif