/* 
 * Author: bye
 * Date: 2023-09-18 15:31:52
 * LastEditors: bye
 * LastEditTime: 2023-09-23 15:25:57
 * FilePath: /study/RVOS/code/myRVOS/08_preemptive/user.c
 * Description: os_main, 用户任务
 */

#include "os.h" 

extern void trap_test(void);

void user_task0() {
    uart_puts("Task 0: Created!...\n");
    task_yield();
    uart_puts("Task 0: I'm back...\n");
    while (1) {
        uart_puts("Task 0: Running... \n");
        task_delay(1000);
    }
}

void user_tesk1() {
    uart_puts("Task 1: Created!\n");
    while (1) {
        uart_puts("Task 1: Running... \n");
        task_delay(1000);
    }
}

void os_main() {
    task_create(user_task0);
    task_create(user_tesk1);
}