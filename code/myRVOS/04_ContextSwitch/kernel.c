/* 
 * Author: bye
 * Date: 2023-09-12 15:18:15
 * LastEditors: bye
 * LastEditTime: 2023-09-18 16:08:13
 * FilePath: /study/RVOS/code/myRVOS/04_ContextSwitch/kernel.c
 * Description: 
 */
#include "os.h"

void start_kernel(void) {
    uart_init();
    uart_puts("hello, rvos~\n");

    page_init();
    sched_init();
    os_main();
    schedule();
    
    uart_puts("Would not go here! \n");
}