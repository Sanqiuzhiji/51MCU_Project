#ifndef TIMER_LED_TEST_H
#define TIMER_LED_TEST_H

/*
 * 使用 Timer0 周期中断，交替驱动 P3.1/TXD 与 P3.0/RXD 上的指示灯。
 * 进入该模式后，这两个引脚不再具备串口功能。
 */
void timer_led_test_init(void);

#endif /* TIMER_LED_TEST_H */
