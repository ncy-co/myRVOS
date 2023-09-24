/* 
 * Author: bye
 * Date: 2023-09-18 15:31:52
 * LastEditors: bye
 * LastEditTime: 2023-09-24 11:57:40
 * FilePath: /study/RVOS/code/myRVOS/10_syscall/user.c
 * Description: os_main, 用户任务
 */

#include "os.h" 
#include "user_api.h"

// #define USE_LOCK
#define MAX_USER 10

extern void trap_test(void);

static userdata user_list[MAX_USER];
static char user_str[MAX_USER][100];

void func(void *arg) {
    if (arg == NULL) return;

    int num = ((userdata *)arg)->num;
    char *str = ((userdata *)arg)->str;
    printf("Number %d: %s\n", num, str);
}

void user_task0() {
    uart_puts("Task 0: Created!...\n");
    // task_yield();
    // uart_puts("Task 0: I'm back...\n");

    // timer_create(func, &user_list[0], 1);

    unsigned int hid = -1;

    #ifdef CONFIG_SYSCALL
        int ret = gethartid(&hid);
        if (!ret) {
            printf("System call returned! Hartid is %d\n", ret);
        }else {
            printf("gethartid() failed! Fault code : %d\n", ret);
        }
    #endif


    while (1) {
        // 上锁
        #ifdef USE_LOCK
                spin_lock();
        #endif

        uart_puts("Task 0: Begin... \n");
        for (int i = 0; i < 5; i++ ) {
            uart_puts("Task 0: Running...\n");
            task_delay(1000);
        }
        uart_puts("Task 0: End ... \n");
        // 释放锁
        #ifdef USE_LOCK
                spin_unlock();
        #endif
    }
}

void user_tesk1() {
    // timer_create(func, &user_list[1], 2);
    uart_puts("Task 1: Created!\n");
    while (1) {
        #ifdef USE_LOCK
                spin_lock();
        #endif

        uart_puts("Task 1: Begin... \n");
        for (int i = 0; i < 5; i++ ) {
            uart_puts("Task 1: Running...\n");
            task_delay(1000);
        }
        uart_puts("Task 1: End ... \n");
        // 释放锁
        #ifdef USE_LOCK
                spin_unlock();
        #endif
    }
}

void os_main() {
    // 输入 task 数据
    user_list[0].num = 0; 
    user_str[0][0] = 'N';
    user_str[0][1] = 'o';
    user_str[0][2] = '.';
    user_str[0][3] = '0';
    user_str[0][4] = '\0';
    user_list[0].str = user_str[0];

    // 注册 task 上下文
    task_create(user_task0);

    user_list[1].num = 1;
    user_str[1][0] = 'N';
    user_str[1][1] = 'o';
    user_str[1][2] = '.';
    user_str[1][3] = '1';
    user_str[1][4] = '\0';
    user_list[1].str = user_str[1];
    task_create(user_tesk1);
}