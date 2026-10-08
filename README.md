# 51MCU_Project

本工程包含两种互斥的板级测试：

- `APP_TEST_MODE_UART`：P3.1/TXD、P3.0/RXD 用作串口，执行阻塞式回显测试；Timer1 产生 9600 波特率。
- `APP_TEST_MODE_TIMER_LED`：P3.1/TXD、P3.0/RXD 用作 LED 输出；Timer0 每 10 ms 中断，每隔 500 ms 用 100 ms 短脉冲触发一次 LED，TX/RX 指示灯轮流闪烁。

## 选择测试模式

编辑 `include/app_config.h` 中的这一行，然后重新编译、烧录：

```c
/* 串口测试 */
#define APP_TEST_MODE APP_TEST_MODE_UART

/* 或者：Timer + LED 测试 */
#define APP_TEST_MODE APP_TEST_MODE_TIMER_LED
```

两个模式共用 P3.0/P3.1，不能同时运行。LED 默认按低电平点亮处理；如果开发板上的 LED 是高电平点亮，将 `include/timer_led_config.h` 中的 `TIMER_LED_ACTIVE_LOW` 改为 `0`。

如果 P3.0/RXD 同时由板载 USB-UART 芯片主动驱动，单片机不能安全地强行控制这一路。此时应断开 USB-UART 的 TX 输出或串联跳线后再做双灯 GPIO 测试；否则只能由单片机可靠控制 P3.1/TXD 对应的活动灯。
