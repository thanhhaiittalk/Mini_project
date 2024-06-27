# Add the source files in this directory
set(SRC_FILES
    exit.cpp
)

# Create a library from these files
add_library(exit_lib ${SRC_FILES})

# Specify include directories for this target
target_include_directories(exit_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/exit)