#include "app_config.h"

#if APP_TEST_MODE == APP_TEST_MODE_UART
#include "uart.h"
#else
#include "timer_led_test.h"
#endif

/*
 * 串口回显示例
 *
 * 接线：
 *   P3.1 / TXD -> USB-UART RX
 *   P3.0 / RXD <- USB-UART TX
 *   GND         -> USB-UART GND
 *
 * 串口终端的数据格式固定为 8N1；波特率在 uart_config.h 中配置。
 */
void main(void)
{
#if APP_TEST_MODE == APP_TEST_MODE_UART
    char received;

    uart_init();
    uart_puts("\r\n8051 UART test ready.\r\n");
    uart_puts("Type characters; the MCU will echo them.\r\n> ");

    /*
     * 应用层只关心“接收”和“发送”，不需要了解底层寄存器。
     * 这种结构便于以后将回显逻辑替换为命令解析或通信协议。
     */
    while (1)
    {
        received = uart_getc();
        uart_putc(received);

        if (received == '\r')
        {
            uart_putc('\n');
            uart_puts("> ");
        }
    }
#else
    /*
     * Timer0 中断产生短脉冲，使 TX/RX 指示灯轮流闪烁。
     * 主循环无需延时，LED 的节拍完全由硬件定时器产生。
     */
    timer_led_test_init();
    while (1)
    {
    }
#endif

}
