// /* 
//  * Author: bye
//  * Date: 2023-09-18 10:21:08
//  * LastEditors: bye
//  * LastEditTime: 2023-09-19 11:31:09
//  * FilePath: /study/RVOS/code/myRVOS/04_ContextSwitch/sched.c
//  * Description: 执行任务的计划
//  */

// #include "os.h"

// extern void switch_to(context *next);

// #define MAX_TASKS 10
// #define STACK_SIZE 1024

// // 所有任务的栈
// uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
// // 所有任务的上下文
// context ctx_tasks[MAX_TASKS];
// // _cur_max_tasks 当前最大的任务数
// // _cur_task 当前任务标号
// static int _num_tasks = 0;
// static int _cur_task = -1;

// static void _set_mscratch(uint32_t x) {
//     asm (
//         "csrw mscratch, %0"
//         :
//         :"r"(x)
//     );
// }

// void sched_init() {
//     // 初始化当前任务的上下文寄存器, 以及mscratch寄存器
//     // 设置 当前上下文为0号任务上下文
//     _set_mscratch(0);
// }

// // 执行下一个任务
// void schedule() {
//     if (_num_tasks <= 0) {
//         printf("Error: The available tasks are 0\n");
//     }
//     _cur_task = (_cur_task + 1) % _num_tasks;
//     context *next = &(ctx_tasks[_cur_task]);
//     // 进行上下文切换
//     switch_to(next);
// }

// // 成功返回-1，失败返回0
// int task_create(void (*address_task)(void)) {
//     if (_num_tasks < MAX_TASKS) {
        
//         ctx_tasks[_num_tasks].sp = (uint32_t)&task_stack[_num_tasks];
//         ctx_tasks[_num_tasks].ra = (uint32_t)address_task;
//         _num_tasks++;
//         return 0;
//     }else return -1;
// }

// void task_yield() {
//     schedule();
// }

// void task_delay(volatile int count) {
//     count *= 50000;
//     while (count--);
// }


#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024
/*
 * In the standard RISC-V calling convention, the stack pointer sp
 * is always 16-byte aligned.
 */
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

void sched_init()
{
	w_mscratch(0);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	if (_top <= 0) {
		printf("Num of task should be greater than zero!\n");
		return;
	}

	_current = (_current + 1) % _top;
	struct context *next = &(ctx_tasks[_current]);
	switch_to(next);
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routine entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*start_routin)(void))
{
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE];
		ctx_tasks[_top].ra = (reg_t) start_routin;
		_top++;
		return 0;
	} else {
		return -1;
	}
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
	schedule();
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

