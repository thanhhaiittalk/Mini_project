# Add the source files in this directory
set(SRC_FILES
    communication.c
)

# Create a library from these files
add_library(communication_lib ${SRC_FILES})

# Specify include directories for this target
target_include_directories(communication_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/communication)