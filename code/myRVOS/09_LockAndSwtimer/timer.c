/* 
 * Author: bye
 * Date: 2023-09-22 18:50:33
 * LastEditors: bye
 * LastEditTime: 2023-09-23 20:28:38
 * FilePath: /study/RVOS/code/myRVOS/09_lock/timer.c
 * Description: 硬件定时器相关设置
 */

#include "os.h"

// 时间间隔为一秒
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ
// 软件定时器最多为10个
#define MAX_TIMER 10

extern void schedule(void);

// 软件定时器数组
static Timer _timer_list[MAX_TIMER];

// _tick ：时钟中断的次数（一秒中断一次）
static uint32_t _tick = 0;

// 初始化 mtimecmp 寄存器
void time_load(int interval) {
    int hartid = _r_mhartid();
    *(uint64_t *)CLINT_MTIMECMP(hartid) = interval + *(uint64_t *)CLINT_MTIME;
}

void timer_init() {
    // 初始化 软件定时器 列表
    for (int i = 0; i < MAX_TIMER; i++ ) {
        _timer_list[i].arg = NULL;
        _timer_list[i].func = NULL;
    }

    // 初始化 mtimecmp 寄存器，一秒中断一次
    time_load(TIMER_INTERVAL);

    // enable interrupt
    _w_mie(_r_mie() | MIE_MTIE);
}

void timer_handler() {
    // 秒数 + 1， 时钟中断次数 + 1
    _tick++;

    // 判断软件定时器列表中哪些软件定时器超时
    for (int i = 0; i < MAX_TIMER; i++ ) {
        if (NULL != _timer_list[i].func) {
            if (_timer_list[i].timeout_tick <= _tick) {
                printf("Software timer %d: Timer Interrupt!\n", i);
                // 执行 时钟中断处理函数
                _timer_list[i].func(_timer_list[i].arg);
                _timer_list[i].func = NULL;
                _timer_list[i].arg = NULL;
                break;
            }
        }
    }

    // 重置时钟中断
    time_load(TIMER_INTERVAL);

    schedule();
}

Timer *timer_create(void (* func)(void *arg), void *arg, uint32_t interval) {
    int t = 0;
    spin_lock();
    while (_timer_list[t].arg != NULL && t < MAX_TIMER) {
        t++;
    }
    if (t >= MAX_TIMER) {
        spin_unlock();
        return NULL;
    }
    _timer_list[t].arg = arg;
    _timer_list[t].func = func;
    _timer_list[t].timeout_tick = _tick + interval;
    spin_unlock();
    return &_timer_list[t];
}

void timer_delete(Timer * timer) {
    if (timer == NULL) return;
    spin_lock();
    timer->arg = NULL;
    timer->func = NULL;
    spin_unlock();
}