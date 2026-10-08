cmake_minimum_required(VERSION 3.20)

project(51MCU_Project C)

# ============================================================
# 编译器配置：使用 SDCC 为 8051 目标生成固件
# ============================================================

find_program(
        SDCC_EXECUTABLE
        sdcc
        REQUIRED
)

message(STATUS "SDCC: ${SDCC_EXECUTABLE}")

# ============================================================
# 源文件和编译产物
# ============================================================

set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/uart.c
        ${CMAKE_SOURCE_DIR}/src/timer_led_test.c
)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/firmware)
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/objects)

set(MAIN_OBJECT ${OBJECT_DIR}/main.rel)
set(UART_OBJECT ${OBJECT_DIR}/uart.rel)
set(TIMER_LED_OBJECT ${OBJECT_DIR}/timer_led_test.rel)
set(FIRMWARE ${OUTPUT_DIR}/firmware.ihx)

# ============================================================
# 分别编译每个模块
# ============================================================

# SDCC 一次只能编译一个 C 源文件，因此每个模块有独立的编译命令。
add_custom_command(
        OUTPUT ${MAIN_OBJECT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJECT_DIR}
        COMMAND ${SDCC_EXECUTABLE}
                -mmcs51
                --model-small
                -I${CMAKE_SOURCE_DIR}/include
                -c
                -o ${MAIN_OBJECT}
                ${CMAKE_SOURCE_DIR}/src/main.c
        DEPENDS
                ${CMAKE_SOURCE_DIR}/src/main.c
                ${CMAKE_SOURCE_DIR}/include/app_config.h
                ${CMAKE_SOURCE_DIR}/include/uart.h
                ${CMAKE_SOURCE_DIR}/include/timer_led_test.h
        COMMENT "Compiling main.c..."
        VERBATIM
)

add_custom_command(
        OUTPUT ${UART_OBJECT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJECT_DIR}
        COMMAND ${SDCC_EXECUTABLE}
                -mmcs51
                --model-small
                -I${CMAKE_SOURCE_DIR}/include
                -c
                -o ${UART_OBJECT}
                ${CMAKE_SOURCE_DIR}/src/uart.c
        DEPENDS
                ${CMAKE_SOURCE_DIR}/src/uart.c
                ${CMAKE_SOURCE_DIR}/include/uart.h
                ${CMAKE_SOURCE_DIR}/include/uart_config.h
        COMMENT "Compiling uart.c..."
        VERBATIM
)

add_custom_command(
        OUTPUT ${TIMER_LED_OBJECT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJECT_DIR}
        COMMAND ${SDCC_EXECUTABLE}
                -mmcs51
                --model-small
                -I${CMAKE_SOURCE_DIR}/include
                -c
                -o ${TIMER_LED_OBJECT}
                ${CMAKE_SOURCE_DIR}/src/timer_led_test.c
        DEPENDS
                ${CMAKE_SOURCE_DIR}/src/timer_led_test.c
                ${CMAKE_SOURCE_DIR}/include/timer_led_test.h
                ${CMAKE_SOURCE_DIR}/include/timer_led_config.h
        COMMENT "Compiling timer_led_test.c..."
        VERBATIM
)

# ============================================================
# 链接固件
# ============================================================

add_custom_command(
        OUTPUT ${FIRMWARE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        COMMAND ${SDCC_EXECUTABLE}
                -mmcs51
                --model-small
                -o ${FIRMWARE}
                ${MAIN_OBJECT}
                ${UART_OBJECT}
                ${TIMER_LED_OBJECT}
        DEPENDS
                ${MAIN_OBJECT}
                ${UART_OBJECT}
                ${TIMER_LED_OBJECT}
        COMMENT "Linking 8051 firmware..."
        VERBATIM
)

add_custom_target(
        firmware ALL
        DEPENDS ${FIRMWARE}
        # 显式列出源文件和头文件，方便在 CLion 中浏览和跳转。
        SOURCES
                ${SOURCES}
                ${CMAKE_SOURCE_DIR}/include/uart.h
                ${CMAKE_SOURCE_DIR}/include/uart_config.h
                ${CMAKE_SOURCE_DIR}/include/app_config.h
                ${CMAKE_SOURCE_DIR}/include/timer_led_test.h
                ${CMAKE_SOURCE_DIR}/include/timer_led_config.h
)

# ============================================================
# 使用 stcgal 烧录 STC 单片机
# ============================================================

set(
        STCGAL_PYTHON
        # 使用虚拟环境中的 Python，避免工程改名导致 stcgal 启动器失效。
        ${CMAKE_SOURCE_DIR}/.venv/Scripts/python.exe
)

# 烧录端口不是编译选项，不放进 CMakeCache。
# 修改此处后只需重新加载 CMake，无需“清除缓存并重新加载”。
set(STC_PORT COM5)

add_custom_target(
        flash
        COMMAND
                ${STCGAL_PYTHON}
                -m stcgal
                -p ${STC_PORT}
                ${FIRMWARE}
        DEPENDS firmware
        COMMENT "Flashing firmware to STC MCU on ${STC_PORT}..."
        USES_TERMINAL
)
