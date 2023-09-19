/* 
 * Author: bye
 * Date: 2023-09-13 08:58:01
 * LastEditors: bye
 * LastEditTime: 2023-09-14 15:58:52
 * FilePath: /study/RVOS/code/myRVOS/MeManagement/printf.c
 * Description: qemu gcc 无 stdio.h,需自己实现printf
 */

#include "os.h"

// va_list :可变参数   #include <stdarg.h>
// out: 存放进行格式转换之后的字符串（将%d转换为特定的整数）
// n：out缓冲区大小（包含 \0 ）
// 返回值为转换后的字符串长度（不包含\0）
static int _vts_printf(char *out, size_t n, const char *s, va_list v1) {
    // 是否为特殊格式
    int format = 0;
    // 是否是长整型格式
    int longarg = 0;
    size_t pos = 0;
    while (*s) {
        // 该字符表示特殊格式 %ld %d %p %x %s %c 
        if (format) {
            switch (*s) {
            case 'l': {
                // 处理%ld格式
                longarg = 1;
                break;
            }
            // 交由下面的 case x 处理 
            case 'p' : {
                longarg = 1;
                // 先输入0x
                if (out && pos < n) {
                    out[pos] = '0';
                }
                pos++;
                if (out && pos < n) {
                    out[pos] = 'x';
                }
                pos++;
            }
            case 'x' : {
                // 将数字解包处理
                long num = longarg ? va_arg(v1, long) : va_arg(v1, int);
                // 负数判断
                if (num < 0) {
                    // 写入到out之前，需判断是否发生缓存区溢出
                    // 若pos未越界，则直接放入out中，反之，则不处理
                    if (out && pos < n) {
                        out[pos] = '-';
                    }
                    // pos用于统计 进行格式转换之后的字符串的总长度
                    pos++;
                }
                int digit = 2 * (longarg ? sizeof(long) : sizeof(int));
                for (int i = digit - 1; i >= 0; i-- ) {
                    long t = (num >> (4 * i)) & 0xf;
                    if (out && pos < n) {
                        out[pos] = t < 10 ? ('0' + t) : ('A' + t - 10); 
                    }
                    pos++;
                }
                // %d/%ld处理结束，将标志format，longarg初始化
                format = 0;
                longarg = 0;
                break;
            }
            case 'd': {
                long num = longarg ? va_arg(v1, long) : va_arg(v1, int);
                if (num < 0) {
                    if (out && pos < n) 
                        out[pos] = '-';
                    pos++;
                    num = -num;
                }
                int digit = 0;
                // 统计num是几位数
                if (num != 0) 
                    for (long t = num; t; digit++) t /= 10;
                else digit = 1;
                for (int i = digit - 1; i >= 0; i-- ) {
                    if (out && (pos + i < n)) {
                        out[pos + i] = '0' + (num % 10);
                    }
                    num /= 10;
                }
                pos += digit;
                format = 0;
                longarg = 0;
                break;
            }
            case 's' : {
                // 字符串，则解包解出指针
                const char *str = va_arg(v1, const char *);
                while (*str) {
                    if (out && pos < n) {
                        out[pos] = *s;
                    }
                    pos++;
                    str++;
                }
                format = 0;
                longarg = 0;
                break;
            }
            case 'c' : {
                const char *ch = va_arg(v1, const char *);
                if (out && pos < n) { 
                    out[pos] = *ch;
                }
                pos++;
                format = 0;
                longarg = 0;
                break;
            }
            default:
                break;
            }
        } else if (*s == '%') {
            // 表示下一个字符为特殊格式
            format = 1;
        }else {
            if (out && pos < n) {
                out[pos] = *s;
            }
            pos++;
        }
        s++;
    }
    // 将 \0 输入到字符串末尾
    if (out && pos < n) 
        out[pos] = '\0';
    else if (out) 
        out[n - 1] = 0; 
    return pos;
}

static char out_buf[1000];

// 返回输出字符串的长度 (不包含 \0)
static int _vprintf(const char *s, va_list vl) {
    int res = _vts_printf(NULL, -1, s, vl);
    // 判断转换后的字符串能否放入到out_buf中, +1：有 \0
    if (res + 1 > sizeof(out_buf)) {
        // 输出溢出信息
        uart_puts("error: output string size overflow\n");
    }
    _vts_printf(out_buf, res + 1, s, vl);
    uart_puts(out_buf);
    return res;
}

// 可变参数，先定义va_list, 再采用va_start，va_arg(vl, int) 解包, 最终采用va_end释放空间
int printf(const char *s, ...) {
    int res = 0;
    va_list vl;
    // vl为定义的va_list, s为 首个可变参数前 的参数名
    va_start(vl, s);
    res = _vprintf(s, vl);
    va_end(vl);
    return res;
}

void printc(char *s) {
    printf("%c", *s);
    printf("\n");
}
