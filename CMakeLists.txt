cmake_minimum_required(VERSION 3.5)
project(mini_project VERSION 1.0)

# Add preprocessor definitions
add_definitions(-DDEBUG)

# Set toolchain
set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/communication
    ${CMAKE_SOURCE_DIR}/src/log
    ${CMAKE_SOURCE_DIR}/src/sensor
)

# Add subdirectories
add_subdirectory(src/communication)
add_subdirectory(src/log)
add_subdirectory(src/sensor)

# Create the executable for 2 processes
add_executable(communication src/communication/communication.c)
add_executable(sensor src/sensor/sensor.c)
add_executable(log src/log/log.c)
