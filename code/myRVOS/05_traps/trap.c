/* 
 * Author: bye
 * Date: 2023-09-19 19:05:31
 * LastEditors: bye
 * LastEditTime: 2023-09-20 16:07:34
 * FilePath: /study/RVOS/code/myRVOS/05_traps/trap.c
 * Description: 
 */

#include "os.h"

extern void trap_vector(void);

// 设置trap处理程序的入口地址
// trap处理程序负责保存上下文，恢复上下文等
void trap_init() {
    _w_mtvec((reg_t)trap_vector);
}

// 具体的trap处理方法
reg_t trap_handler(reg_t epc, reg_t cause) {
    reg_t return_epc = epc;
    reg_t trap_code = cause & ((reg_t)(1 << 31) - 1);
    if (cause & (1 << 31)) {
        // 此时trap为interrupt
        switch (trap_code) {
        case 3:
            uart_puts("Software interrupt\n");
            break;
        case 7:
            uart_puts("Timer interrupt\n");
            break;
        case 11:
            uart_puts("External interrupt\n");
            break;
        default:
            uart_puts("Unknown async exception\n");
            break;
        }

    }else {
        // 此时trap为exception
        printf("Sync exceptions! code = %d\n", trap_code);
        return_epc -= 4;
    }

    // 返回mepc
    return return_epc;

}

void trap_test() {
    // Store/AMO access fault
    *(int *)(0x00000) = 100;

    uart_puts("I'm return back from trap!\n");
}

