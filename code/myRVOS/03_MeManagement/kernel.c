/* 
 * Author: bye
 * Date: 2023-09-12 15:18:15
 * LastEditors: bye
 * LastEditTime: 2023-09-14 09:54:37
 * FilePath: /study/RVOS/code/myRVOS/MeManagement/kernel.c
 * Description: 
 */
#include "os.h"

void start_kernel(void) {
    uart_init();
    uart_puts("hello, rvos~\n");

    page_init();
    page_test();
    
}