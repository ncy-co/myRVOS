/* 
 * Author: bye
 * Date: 2023-09-22 18:50:33
 * LastEditors: bye
 * LastEditTime: 2023-09-22 19:21:00
 * FilePath: /study/RVOS/code/myRVOS/07_hardwareTimer/timer.c
 * Description: 硬件定时器相关设置
 */

#include "os.h"

// 时间间隔为一秒
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

// _tick ：时钟中断的次数（一秒中断一次）
static uint32_t _tick = 0;

// 初始化 mtimecmp 寄存器
void time_load(int interval) {
    int hartid = _r_mhartid();
    *(uint64_t *)CLINT_MTIMECMP(hartid) = interval + *(uint64_t *)CLINT_MTIME;
}

void timer_init() {
    // 初始化 mtimecmp 寄存器，一秒中断一次
    time_load(TIMER_INTERVAL);

    // enable interrupt
    _w_mstatus(_r_mstatus() | MSTATUS_MIE);
    _w_mie(_r_mie() | MIE_MTIE);
    
}

void timer_handler() {
    // 秒数 + 1， 时钟中断次数 + 1
    _tick++;

    printf("tick: %d \n", _tick);

    // 重置时钟中断
    time_load(TIMER_INTERVAL);
}