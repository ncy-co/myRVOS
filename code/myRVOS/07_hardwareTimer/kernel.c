/* 
 * Author: bye
 * Date: 2023-09-12 15:18:15
 * LastEditors: bye
 * LastEditTime: 2023-09-22 19:21:52
 * FilePath: /study/RVOS/code/myRVOS/07_hardwareTimer/kernel.c
 * Description: 
 */
#include "os.h"

extern void os_main(void);
extern void schedule(void);
extern void trap_init(void);
extern void timer_init(void);

void start_kernel(void) {
    uart_init();
    uart_puts("hello, rvos~\n");

    page_init();
    
    trap_init();

    plic_init();

    sched_init();

    timer_init();

    os_main();

    schedule();
    
    uart_puts("Would not go here! \n");
}