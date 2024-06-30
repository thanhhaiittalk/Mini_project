#ifndef MP_SHARED_MEMORY_HPP
#define MP_SHARED_MEMORY_HPP

#include <iostream>  // Standard library includes
#include <sys/mman.h>

// Forward declarations (if necessary)
// class OtherClass;

class MPSharedMemoryObject {
public:
    MPSharedMemoryObject(const char* shmName, int oflag, mode_t mode);  // Parameterized constructor
    ~MPSharedMemoryObject();  // Destructor
    void truncate(off_t length);
    static void remove (const char* name);
    
    int shmFd;

private:
    const char* shmName;

};

#endif // MYCLASS_HPP
