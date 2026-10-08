#ifndef TIMER_LED_CONFIG_H
#define TIMER_LED_CONFIG_H

/* 适用于标准 12T 8051。Timer0 每 10 ms 中断一次。 */
#define TIMER_LED_FOSC_HZ          11059200UL
#define TIMER_LED_TICK_HZ          100UL

/* 每 500 ms 触发一次 LED，并用 100 ms 短脉冲模拟串口收发活动。 */
#define TIMER_LED_EVENT_TICKS      50U
#define TIMER_LED_PULSE_TICKS      10U

/* 常见串口收发指示灯为低电平点亮；若板卡相反，改成 0。 */
#define TIMER_LED_ACTIVE_LOW       1

#endif /* TIMER_LED_CONFIG_H */
