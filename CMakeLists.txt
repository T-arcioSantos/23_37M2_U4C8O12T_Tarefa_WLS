# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================




cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)
project(23_37M2_U4C8O12T_Tarefa_WLS C CXX ASM)
pico_sdk_init()
add_executable(23_37M2_U4C8O12T_Tarefa_WLS 23_37M2_U4C8O12T_Tarefa_WLS.c lib/ssd1306.c)
pico_set_program_name(23_37M2_U4C8O12T_Tarefa_WLS "23_37M2_U4C8O12T_Tarefa_WLS")
pico_set_program_version(23_37M2_U4C8O12T_Tarefa_WLS "0.1")
pico_enable_stdio_uart(23_37M2_U4C8O12T_Tarefa_WLS 1)
pico_enable_stdio_usb(23_37M2_U4C8O12T_Tarefa_WLS 1)
target_link_libraries(23_37M2_U4C8O12T_Tarefa_WLS pico_stdlib hardware_i2c hardware_adc hardware_pwm)
target_include_directories(23_37M2_U4C8O12T_Tarefa_WLS PRIVATE   ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(23_37M2_U4C8O12T_Tarefa_WLS)

