/* 
 * Author: bye
 * Date: 2023-09-23 17:06:40
 * LastEditors: bye
 * LastEditTime: 2023-09-23 17:08:20
 * FilePath: /study/RVOS/code/myRVOS/09_lock/lock.c
 * Description: 
 */
#include "os.h"

int spin_lock() {
    _w_mstatus(_r_mstatus() & (~MSTATUS_MIE));
    return 0;
}

int spin_unlock() {
    _w_mstatus(_r_mstatus() | (MSTATUS_MIE));
    return 0;
}
