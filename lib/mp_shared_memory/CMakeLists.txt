# Add the source files in this directory
set(MP_SHARED_MEMORY_FILES
    mp_shared_memory_object.cpp
    mp_shared_memory_object.hpp
    unordered_map_shm.cpp 
    unordered_map_shm.hpp
)

# Create a library from these files
add_library(mp_shared_memory_lib ${MP_SHARED_MEMORY_FILES})

# Specify include directories for this target
target_include_directories(mp_shared_memory_lib PUBLIC ${CMAKE_SOURCE_DIR}/lib/mp_shared_memory)
target_link_libraries(mp_shared_memory_lib PUBLIC rt)