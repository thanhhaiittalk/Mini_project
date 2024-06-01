# Add the source files in this directory
set(SRC_FILES
    log.c
)

# Create a library from these files
add_library(log_lib ${SRC_FILES})

# Specify include directories for this target
target_include_directories(log_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/log)