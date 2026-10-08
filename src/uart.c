#include <8051.h>

#include "uart.h"
#include "uart_config.h"

/*
 * 串口工作在模式 1，波特率由定时器 1 提供。
 * 定时器 1 使用模式 2（8 位自动重装），经典 12T 8051 的计算公式为：
 *
 *   波特率 = 晶振频率 / 12 / 32 / (256 - TH1)
 *
 * 11.0592 MHz 晶振和 9600 波特率对应的重装值为 253（0xFD）。
 */
#define UART_TIMER1_RELOAD \
    (256UL - (UART_FOSC_HZ / 12UL / 32UL / UART_BAUD_RATE))

void uart_init(void)
{
    SCON = 0x50;                 /* 串口模式 1，并允许接收。 */
    TMOD = (TMOD & 0x0F) | 0x20; /* 保留定时器 0，定时器 1使用模式 2。 */

    /* TH1 是自动重装值，TL1 是定时器当前计数值。 */
    TH1 = (unsigned char)UART_TIMER1_RELOAD;
    TL1 = (unsigned char)UART_TIMER1_RELOAD;

    TR1 = 1; /* 启动定时器 1，开始产生串口波特率。 */
    TI = 0;  /* 清除发送完成标志。 */
    RI = 0;  /* 清除接收完成标志。 */
}

void uart_putc(char value)
{
    SBUF = value;

    /* 硬件发送完一个完整的数据帧后会将 TI 置 1。 */
    while (!TI)
    {
    }
    TI = 0;
}

char uart_getc(void)
{
    char value;

    /* 硬件收到一个完整的数据帧后会将 RI 置 1。 */
    while (!RI)
    {
    }

    RI = 0;
    value = SBUF;
    return value;
}

void uart_puts(const char *text)
{
    while (*text != '\0')
    {
        uart_putc(*text);
        ++text;
    }
}
