/* 
 * Author: bye
 * Date: 2023-09-24 09:57:49
 * LastEditors: bye
 * LastEditTime: 2023-09-24 10:40:05
 * FilePath: /study/RVOS/code/myRVOS/10_syscall/syscall.c
 * Description: 
 */
#include "os.h"
#include "syscall.h"

// 执行系统调用
void do_syscall (context *cxt) {
    // 获取具体的系统调用编号
    uint32_t syscall_num = cxt->a7;
    switch (syscall_num) {
    case SYS_gethartid:
        // a0 表示是否执行成功
        if ((uint32_t *)cxt->a0 == NULL) cxt->a0 = -1;
        else {
            // 将返回值放入在 hid 中（cxt->a0 中存放的是hid的地址
            *(uint32_t *)cxt->a0 = _r_mhartid();
            // a0为该函数的返回值，
            cxt->a0 = 0;
        }
        break;
    
    default:
        printf("Unknown syscall no: %d\n", syscall_num);
        cxt->a0 = -1;
    }
}