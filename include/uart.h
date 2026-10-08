#ifndef UART_H
#define UART_H

/* 初始化串口。使用前必须调用一次。 */
void uart_init(void);

/* 阻塞发送一个字符：函数会等待字符发送完成后再返回。 */
void uart_putc(char value);

/* 阻塞接收一个字符：函数会等待收到数据后再返回。 */
char uart_getc(void);

/* 发送以 '\0' 结尾的字符串。 */
void uart_puts(const char *text);

#endif /* UART_H */
