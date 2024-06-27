#include "mp_shared_memory_object.hpp"
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

// Parameterized constructor
MPSharedMemoryObject::MPSharedMemoryObject(const char* name, int oflag, mode_t mode) : shmName(name){
    shmFd = shm_open (name, oflag, mode);
    if (shmFd == -1) {
        std::cerr << "Failed to open shared memory" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Destructor
MPSharedMemoryObject::~MPSharedMemoryObject() {}

// Truncate
void MPSharedMemoryObject::truncate(off_t length) {
    // Configure size of the shared memory object
    if (ftruncate(shmFd, length) == -1) {
        std::cerr << "Failed to resize shared memory" << std::endl;
    }
}

// Remove share memory
void MPSharedMemoryObject::remove (const char* name){
    shm_unlink(name);
}

