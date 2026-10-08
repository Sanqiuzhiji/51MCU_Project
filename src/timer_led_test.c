#include <8051.h>

#include "timer_led_config.h"
#include "timer_led_test.h"

#define TIMER0_COUNTS_PER_TICK \
    (TIMER_LED_FOSC_HZ / 12UL / TIMER_LED_TICK_HZ)
#define TIMER0_RELOAD (65536UL - TIMER0_COUNTS_PER_TICK)
#define TIMER0_RELOAD_HIGH ((unsigned char)(TIMER0_RELOAD >> 8))
#define TIMER0_RELOAD_LOW  ((unsigned char)(TIMER0_RELOAD & 0xFFU))

#if (TIMER0_COUNTS_PER_TICK == 0UL) || (TIMER0_COUNTS_PER_TICK > 65535UL)
#error "Timer0 tick does not fit in 16-bit mode"
#endif

#if TIMER_LED_ACTIVE_LOW
#define LED_ON  0
#define LED_OFF 1
#else
#define LED_ON  1
#define LED_OFF 0
#endif

static unsigned char event_tick_count;
static unsigned char pulse_tick_count;
static __bit pulse_active;
static __bit select_tx_led;

static void leds_off(void)
{
    P3_1 = LED_OFF;
    P3_0 = LED_OFF;
}

static void selected_led_on(void)
{
    if (select_tx_led)
    {
        P3_1 = LED_ON;
        P3_0 = LED_OFF;
    }
    else
    {
        P3_1 = LED_OFF;
        P3_0 = LED_ON;
    }
}

static void timer0_reload(void)
{
    TH0 = TIMER0_RELOAD_HIGH;
    TL0 = TIMER0_RELOAD_LOW;
}

void timer_led_test_init(void)
{
    TR0 = 0;
    ET0 = 0;

    /* 保留 Timer1 配置，只将 Timer0 设置为模式 1（16 位定时器）。 */
    TMOD = (TMOD & 0xF0) | 0x01;
    timer0_reload();

    event_tick_count = 0;
    pulse_tick_count = 0;
    pulse_active = 1;
    select_tx_led = 1;
    selected_led_on();

    TF0 = 0;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void timer0_isr(void) __interrupt(1)
{
    timer0_reload();

    if (pulse_active)
    {
        ++pulse_tick_count;
        if (pulse_tick_count >= TIMER_LED_PULSE_TICKS)
        {
            pulse_tick_count = 0;
            pulse_active = 0;
            leds_off();
            select_tx_led = !select_tx_led;
        }
    }
    else
    {
        ++event_tick_count;
        if (event_tick_count >= TIMER_LED_EVENT_TICKS)
        {
            event_tick_count = 0;
            pulse_active = 1;
            selected_led_on();
        }
    }
}
