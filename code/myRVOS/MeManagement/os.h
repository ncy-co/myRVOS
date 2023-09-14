/* 
 * Author: bye
 * Date: 2023-09-13 09:00:03
 * LastEditors: bye
 * LastEditTime: 2023-09-14 15:59:27
 * FilePath: /study/RVOS/code/myRVOS/MeManagement/os.h
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


#endif