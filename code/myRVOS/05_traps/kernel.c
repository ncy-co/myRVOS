/* 
 * Author: bye
 * Date: 2023-09-12 15:18:15
 * LastEditors: bye
 * LastEditTime: 2023-09-20 16:27:22
 * FilePath: /study/RVOS/code/myRVOS/05_traps/kernel.c
 * Description: 
 */
#include "os.h"

extern void os_main(void);
extern void schedule(void);

void start_kernel(void) {
    uart_init();
    uart_puts("hello, rvos~\n");

    page_init();
    trap_init();
    sched_init();
    os_main();
    schedule();
    
    uart_puts("Would not go here! \n");
}