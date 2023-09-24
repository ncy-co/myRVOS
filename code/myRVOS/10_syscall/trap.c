/* 
 * Author: bye
 * Date: 2023-09-19 19:05:31
 * LastEditors: bye
 * LastEditTime: 2023-09-24 10:25:19
 * FilePath: /study/RVOS/code/myRVOS/10_syscall/trap.c
 * Description: 
 */

#include "os.h"

extern void trap_vector(void);
extern void timer_handler(void);
extern void schedule(void);
extern void do_syscall(context *cxt);

// 设置trap处理程序的入口地址
// trap处理程序负责保存上下文，恢复上下文等
void trap_init() {
    _w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler() {
    int number_interrupt = plic_claim();
    if (number_interrupt == UART0_IRQ) {
        while(1) {
            int c = uart_getc();
            if (c == -1) {
                break;
            }else {
                uart_putc((char)c);
                uart_putc('\n');
            }
        }
    }else if (number_interrupt) {
        printf("unexpected interrupt number_interrupt = %d \n", number_interrupt);
    }

    if (number_interrupt) {
        plic_complete(number_interrupt);
    }
}

// 具体的trap处理方法
reg_t trap_handler(reg_t epc, reg_t cause, context *cxt) {
    reg_t return_epc = epc;
    reg_t trap_code = cause & (0xfff);
    if (cause & (0x80000000)) {
        // 此时trap为interrupt
        switch (trap_code) {
        case 3:
            uart_puts("Software interrupt\n");
            int id = _r_mhartid();
            *(uint32_t *)CLINT_MSIP(id) = 0;
            schedule();
            break;
        case 7:
            uart_puts("Timer interrupt\n");
            timer_handler();
            break;
        case 11:
            uart_puts("External interrupt\n");
            external_interrupt_handler();
            break;
        default:
            uart_puts("Unknown async exception\n");
            break;
        }

    }else {
        // 此时trap为exception
        printf("Sync exceptions! code = %d\n", trap_code);
        switch (trap_code) {
            // 从user模式下执行的ecall
        case 8:
            uart_puts("System call from U-mode!\n");
            do_syscall(cxt);
            return_epc += 4;
            break;
        
        default:
            break;
        }
        
        uint32_t i = 0xfffffff;
        while (i--) {}
    }

    // 返回mepc
    return return_epc;

}

void trap_test() {
    // Store/AMO access fault
    *(int *)(0x00000) = 100;

    uart_puts("I'm return back from trap!\n");
}

