cmake_minimum_required(VERSION 3.5)
project(mini_project VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add preprocessor definitions
add_definitions(-DDEBUG)

# Set toolchain
# set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/communication
    ${CMAKE_SOURCE_DIR}/src/log
    ${CMAKE_SOURCE_DIR}/src/sensor
    ${CMAKE_SOURCE_DIR}/src/setup
    ${CMAKE_SOURCE_DIR}/src/exit
    ${CMAKE_SOURCE_DIR}/lib/mp_shared_memory
)

# Add subdirectories
add_subdirectory(src/communication)
add_subdirectory(src/log)
add_subdirectory(src/sensor)
add_subdirectory(src/setup)
add_subdirectory(src/exit)
add_subdirectory(lib/mp_shared_memory)

# Create the executable for 2 processes
add_executable(communication src/communication/communication.cpp)
add_executable(sensor src/sensor/sensor.cpp)
add_executable(log src/log/log.cpp)
add_executable(setup src/setup/setup.cpp)
add_executable(exit src/exit/exit.cpp)

# Link libraries
target_link_libraries(communication PUBLIC pthread rt mp_shared_memory_lib setup_lib)
target_link_libraries(sensor PUBLIC pthread rt mp_shared_memory_lib setup_lib)
target_link_libraries(log PUBLIC pthread rt mp_shared_memory_lib setup_lib)
target_link_libraries(setup PUBLIC rt setup_lib)
target_link_libraries(setup PUBLIC mp_shared_memory_lib setup_lib) 
target_link_libraries(exit PUBLIC pthread rt mp_shared_memory_lib setup_lib)

