# Add the source files in this directory
set(SRC_FILES
    sensor.c
)

# Create a library from these files
add_library(sensor_lib ${SRC_FILES})

# Specify include directories for this target
target_include_directories(sensor_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/sensor)