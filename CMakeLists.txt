cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD waveshare_rp2040_zero CACHE STRING "Board type")

set(MS_RATE_DEFAULT 100 CACHE STRING "Default mouse sample rate")
set(MS_RATE_HOST_CONTROL ON CACHE BOOL "Allow the host to configure the mouse sample rate")

# Pull in Raspberry Pi Pico SDK
set(PICO_SDK_PATH ../pico-sdk)

include(pico_sdk_import.cmake)

if (NOT PICO_SDK_VERSION_STRING VERSION_EQUAL "1.5.1")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(rs-usb-host C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(rs-usb-host src/rs-usb-host.c src/usbin.c src/scancodes.c src/serialout.c)

# print memory usage, enable all warnings
target_link_options(rs-usb-host PRIVATE -Xlinker --print-memory-usage)
target_compile_options(rs-usb-host PRIVATE -Wall -Wextra)

add_compile_definitions(UART_ID=uart1)
add_compile_definitions(TX_PIN=4)
add_compile_definitions(RX_PIN=5)
add_compile_definitions(BAUD_RATE=115200)

add_compile_definitions(PICO_PANIC_FUNCTION=reset)

add_compile_definitions(MS_RATE_DEFAULT=${MS_RATE_DEFAULT})
if (MS_RATE_HOST_CONTROL)
    add_compile_definitions(MS_RATE_HOST_CONTROL)
endif()

pico_set_program_name(rs-usb-host "rs-usb-host")
pico_set_program_version(rs-usb-host "0.1")

pico_enable_stdio_uart(rs-usb-host 1)
pico_enable_stdio_usb(rs-usb-host 0)

target_include_directories(rs-usb-host PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(rs-usb-host pico_stdlib hardware_pio tinyusb_host tinyusb_board)

pico_add_extra_outputs(rs-usb-host)
