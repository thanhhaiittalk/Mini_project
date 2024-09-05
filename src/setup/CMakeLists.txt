# Add the source files in this directory
set(SRC_FILES
    main_setup.cpp
    setup.cpp
    setup.hpp
)

# Create a library from these files
add_library(setup_lib ${SRC_FILES})

# Specify include directories for this target
target_include_directories(setup_lib PUBLIC ${CMAKE_SOURCE_DIR}/src/setup)