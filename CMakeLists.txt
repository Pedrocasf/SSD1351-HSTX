# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(W44Pico2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(W44Pico2
    src/main.c
    src/cart/cart.c
    src/platform/pico2/platform.c
    src/rt/runtime.c
)

pico_set_program_name(W44Pico2 "SSD1351-HSTX")
pico_set_program_version(W44Pico2 "0.1")

# Generate PIO header
#pico_generate_pio_header(SSD1351-HSTX ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
#pico_enable_stdio_uart(SSD1351-HSTX 1)
pico_enable_stdio_usb(W44Pico2 1)

# Add the standard library to the build
target_link_libraries(W44Pico2
        pico_multicore
        pico_stdlib
        pico_sync
)

# Add the standard include files to the build
target_include_directories(W44Pico2 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(W44Pico2
#        hardware_spi
#        hardware_i2c
#        hardware_dma
#        hardware_pio
#        hardware_interp
#        hardware_timer
#        hardware_watchdog
#        hardware_clocks
        )

pico_add_extra_outputs(W44Pico2)

