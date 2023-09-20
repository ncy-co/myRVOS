/* 
 * Author: bye
 * Date: 2023-09-18 15:31:52
 * LastEditors: bye
 * LastEditTime: 2023-09-20 16:22:35
 * FilePath: /study/RVOS/code/myRVOS/05_traps/user.c
 * Description: os_main, 用户任务
 */

#include "os.h" 

extern void trap_test(void);

void user_task0() {
    while (1) {
        uart_puts("Task 0: Running... \n");
        trap_test();
        task_delay(1000);
        task_yield();
    }
}

void user_tesk1() {
    while (1) {
        uart_puts("Task 1: Running... \n");
        trap_test();
        task_delay(1000);
        task_yield();
    }
}

void os_main() {
    task_create(user_task0);
    printf("Task 0: Created! \n");
    task_create(user_tesk1);
    printf("Task 1: Created! \n");
    
}