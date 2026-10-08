#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/*
 * 测试模式选择：每次只启用一种模式，因为两种测试都会使用 P3.0/P3.1。
 * 修改 APP_TEST_MODE 后重新编译、烧录即可。
 */
#define APP_TEST_MODE_UART      1
#define APP_TEST_MODE_TIMER_LED 2

#define APP_TEST_MODE APP_TEST_MODE_TIMER_LED

#if (APP_TEST_MODE != APP_TEST_MODE_UART) && \
    (APP_TEST_MODE != APP_TEST_MODE_TIMER_LED)
#error "APP_TEST_MODE must be UART or TIMER_LED"
#endif

#endif /* APP_CONFIG_H */
