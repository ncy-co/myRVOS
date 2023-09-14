/* 
 * Author: bye
 * Date: 2023-09-12 15:18:15
 * LastEditors: bye
 * LastEditTime: 2023-09-14 16:00:22
 * FilePath: /study/RVOS/code/myRVOS/MeManagement/uart.c
 * Description: 
 */
// This file is used to implement UART

#include "types.h"
#include "platform.h"

// 该宏定义即寄存器地址，reg为寄存器编号，一个寄存器8位
#define UART_REG(reg) (volatile uint8_t *)(UART0 + reg)

// 读模式下，0号寄存器用于保存读取的数据，写模式下，将数据写到0号寄存器中发生出去
#define RHR 0   // Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

// 数据已经准备好，可读
#define LSR_RX_READY (1 << 0)
// 寄存器已空，可写
#define LSR_TX_IDLE (1 << 5)

// 读reg号寄存器中的值
#define uart_read_reg(reg) (*(UART_REG(reg)))
// 将v写入reg号寄存器中
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = v)

void uart_init() {
    // disable interrupts
    uart_write_reg(IER, 0x00);

    // 设置 UART 通信协议的波特率（发送速率）信息
	// 若想 UART 通信协议的输出速率为 38.4k， 则需设置配置信息为0x0003
	// 对照表：pdf：td16550  6页
	// 配置信息均为16位的数，UART 寄存器为八位，则配置信息需要放在两个UART寄存器中
	// DLL[UART寄存器] (divisor latch least) 低位，DLM(divisor latch most) 高位
	// LCR[UART寄存器] ：将lcr寄存器的第七位设置为1，表示使能设置了波特率
	// pdf：td16550  4页
    uint8_t lcr = uart_read_reg(LCR);
    uart_write_reg(LCR, (lcr | 1 << 7));
    uart_write_reg(DLL, 0x03);
    uart_write_reg(DLM, 0x00);

    /* 
	 * Continue setting the asynchronous data communication format.
	 * - number of the word length: 8 bits
	 * - number of stop bits：1 bit when word length is 8 bits
	 * - no parity
	 * - no break control
	 * - disabled baud latch
     * pdf td16550 p4
	 */
    lcr = 0;
    uart_write_reg(LCR, lcr | (3 << 0));
}

// 传输一个字符
int uart_putc(char ch) {
    // 轮询等待，若THR寄存器未准备好，则一直查询
    // LSR bit5 为0表示THR寄存器仍是满的，不可写
    while (((1 << 5) & uart_read_reg(LSR)) == 0);
    return  uart_write_reg(THR, ch);
}

// 传输字符串
void uart_puts(char *s) {
    while ((*s) != '\0') {
        uart_putc(*s);
        s++;
    }
}

char uart_inputc() {
    while ((uart_read_reg(LSR) & 1) == 0);
    return uart_read_reg(RHR);
}