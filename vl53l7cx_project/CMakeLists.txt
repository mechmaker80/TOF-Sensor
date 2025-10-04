cmake_minimum_required(VERSION 3.13)

# Set the Pico SDK path
set(PICO_SDK_PATH "/Users/stuartbrodie/pico/pico-sdk")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(/Users/stuartbrodie/pico/pico-sdk/external/pico_sdk_import.cmake)

project(vl53l7cx_driver)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(vl53l7cx_driver
    main.c
    vl53l7cx_driver.c
)

# Test serial executable
add_executable(test_serial
    test_serial.c
)

# Simple blink test
add_executable(simple_blink
    simple_blink.c
)

# Debug version
add_executable(debug_main
    debug_main.c
)

# Minimal test
add_executable(minimal_test
    minimal_test.c
)

# ST Driver example
add_executable(st_driver_example
    main_st_driver.c
    platform_pico.c
    src/vl53l7cx_api.c
    src/vl53l7cx_plugin_detection_thresholds.c
    src/vl53l7cx_plugin_motion_indicator.c
    src/vl53l7cx_plugin_xtalk.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(vl53l7cx_driver 
    pico_stdlib
    hardware_i2c
    hardware_gpio
)

target_link_libraries(test_serial 
    pico_stdlib
)

target_link_libraries(simple_blink 
    pico_stdlib
    hardware_gpio
)

target_link_libraries(debug_main 
    pico_stdlib
    hardware_i2c
    hardware_gpio
)

target_link_libraries(minimal_test 
    pico_stdlib
    hardware_gpio
)

target_link_libraries(st_driver_example 
    pico_stdlib
    hardware_i2c
    hardware_gpio
)

# Add include directories for ST driver
target_include_directories(st_driver_example PRIVATE 
    inc
    .
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(vl53l7cx_driver)
pico_add_extra_outputs(test_serial)
pico_add_extra_outputs(simple_blink)
pico_add_extra_outputs(debug_main)
pico_add_extra_outputs(minimal_test)
pico_add_extra_outputs(st_driver_example)

# enable usb output, disable uart output
pico_enable_stdio_usb(vl53l7cx_driver 1)
pico_enable_stdio_uart(vl53l7cx_driver 0)
pico_enable_stdio_usb(test_serial 1)
pico_enable_stdio_uart(test_serial 0)
pico_enable_stdio_usb(simple_blink 1)
pico_enable_stdio_uart(simple_blink 0)
pico_enable_stdio_usb(debug_main 1)
pico_enable_stdio_uart(debug_main 0)
pico_enable_stdio_usb(minimal_test 1)
pico_enable_stdio_uart(minimal_test 0)
pico_enable_stdio_usb(st_driver_example 1)
pico_enable_stdio_uart(st_driver_example 0)
