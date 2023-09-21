/* 
 * Author: bye
 * Date: 2023-09-21 14:02:54
 * LastEditors: bye
 * LastEditTime: 2023-09-21 15:01:40
 * FilePath: /study/RVOS/code/myRVOS/06_interrupts/plic.c
 * Description: 中断相关函数
 */
#include "os.h"

void plic_init() {
    // read hartid
    uint32_t hart = _r_tp();

    // set priority of uart
    *(uint32_t *)PLIC_PRIORITY(UART0_IRQ) = 1;

    // Enable uart0
    *(uint32_t *)PLIC_MENABLE(hart) = (1 << UART0_IRQ);

    // set threshold
    *(uint32_t *)PLIC_MTHRESHOLD(hart) = 0;

    // set mie of mstatus
    _w_mstatus(_r_mstatus() | (1 << 3));

    // set meie of mie
    _w_mie(_r_mie() | (1 << 11));

}

// 获取中断源编号
int plic_claim() {
    int hart = _r_tp();
    int number_interrupt = *(uint32_t *)PLIC_MCLAIM(hart);
    return number_interrupt;
}

// 告诉 plic 该中断源编号的中断已经处理完成
void plic_complete(int number_interrupt) {
    int hart = _r_tp();
    *(uint32_t *)PLIC_MCOMPLETE(hart) = number_interrupt;
}