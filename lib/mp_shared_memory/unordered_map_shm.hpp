#ifndef UNORDEREDMAPSHM_HPP
#define UNORDEREDMAPSHM_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "mp_shared_memory_object.hpp"

#define WRITE_PTR       "write_ptr"

class UnorderedMapShm {
public:
    // Define a struct within the class
    struct HashEntry {
        char key[20];
        int value;
    };

    // Define a typedef for the struct
    typedef struct HashEntry HashEntryType;
    // Constructor
    UnorderedMapShm(const MPSharedMemoryObject shmObject, int size, int prot);

    // Destructor
    ~UnorderedMapShm();

    void write(char *key, int value);

    bool read(char *key, int* value);

    bool remove (char *key);

    bool find (char *key);

    static int destroy ();

private:
    int writePtr;
    MPSharedMemoryObject shm;
    int size;
    void* shmPtr;
    
};

#endif // UNORDEREDMAPSHM_HPP
