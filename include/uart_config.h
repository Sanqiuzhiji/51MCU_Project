#ifndef UART_CONFIG_H
#define UART_CONFIG_H

/*
 * UART 用户配置
 *
 * 更换晶振或波特率时，只需要修改本文件，无需改动 uart.c。
 * 当前计算公式适用于标准 12T 8051、定时器 1 模式 2、SMOD = 0。
 */
#define UART_FOSC_HZ   11059200UL
#define UART_BAUD_RATE 9600UL

#endif /* UART_CONFIG_H */
