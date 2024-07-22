#include "unordered_map_shm.hpp"
#include <string.h>

// Constructor
UnorderedMapShm::UnorderedMapShm(const MPSharedMemoryObject shmObject, int size, int prot):shm(shmObject),
                                                                                           size(size)
{
    // Map the shared memory object into the process's address space
    shmPtr = mmap(0, size + sizeof(sem_t), prot, MAP_SHARED, shm.shmFd, 0); // Size of data and semaphore
    if (shmPtr == MAP_FAILED) {
        std::cerr << "Failed to map shared memory" << std::endl;
    }

    sem = static_cast<sem_t*>(shmPtr);

    // Intialize Write pointer and semaphore in first creating shared memory
    HashEntryType *entries = static_cast<HashEntryType *>(shmPtr) + sizeof(sem_t);
    if (strcmp(entries[0].key,WRITE_PTR) != 0) {
        writePtr = 0;
        strcpy(entries[0].key, WRITE_PTR);
        entries[0].value = 0;

        // Initialize an unnamed semaphore in shared memory
        if (sem_init(sem, 1, 1) == -1)
        { // 1 means the semaphore is shared between processes
            std::cerr << "Failed to initialize semaphore" << std::endl;
        }
    }
}

// Destructor
UnorderedMapShm::~UnorderedMapShm() {
    // Cleanup code
    munmap(shmPtr, size);
}

// Template method for writing to the unordered map
void UnorderedMapShm::write(char *key, int value) {
    sem_wait(sem);

    read(WRITE_PTR, &writePtr);
    
    // Initialize the hash table in shared memory
    auto *entries = static_cast<HashEntryType *>(shmPtr) + sizeof(sem_t);
    // First element
    if (writePtr == 0) {
        strcpy(entries[1].key, key);
        entries[1].value = value;
        writePtr++;
        entries[0].value = writePtr;
        strcpy(entries[writePtr].key,key);
        entries[writePtr].value = value;
        if (sem_post(sem) == -1)
        {
            throw std::runtime_error("sem_post failed");
        }
        return;
    }
    
    // Next elements
    for (int i = 1; i <= writePtr; i++) {
        if (strcmp(entries[i].key, key) == 0) {
            // Element presented
            entries[i].value = value;
            if (sem_post(sem) == -1) {
                throw std::runtime_error("sem_post failed");
            }
            return;
        }
    }

    // Element not presented yet
    writePtr++;
    entries[0].value = writePtr;
    strcpy(entries[writePtr].key, key);
    entries[writePtr].value = value;

    read(WRITE_PTR, &writePtr);
    if (sem_post(sem) == -1) {
        throw std::runtime_error("sem_post failed");
    }
}

// Template method for reading from the unordered map
bool UnorderedMapShm::read(char *key, int* value) {
    std::unordered_map<std::string, int> mapShm;
    auto *entries = static_cast<HashEntryType *>(shmPtr) + sizeof(sem_t);
    // Load all elements in shared memory into hash table
    int i = 0;
    do {
        mapShm[entries[i].key] = entries[i].value;
        i++;
    } while (i <= entries[0].value);
    // Find element
    if (mapShm.find(key) == mapShm.end()) {
        // Element not present yet
        *value = 0;
        return false;
    }
    else {
        // Element present
        *value = mapShm[key]; 
        return true;
    }

}

bool UnorderedMapShm::remove(char *key) {
    auto *entries = static_cast<HashEntryType *>(shmPtr) + sizeof(sem_t);
    for (int i = 1; i <= entries[0].value; i++) {
        if (key == entries[i].key) {
            // Last element
            if (i == entries[0].value) {
                strcpy(entries[i].key,"");
                entries[i].value = 0;
                entries[0].value--;
                writePtr--;
                return true;
            }

            for (int j = i; j < entries[0].value; j++) {
                strcpy(entries[j].key, entries[j+1].key);
                entries[j].value = entries[j+1].value;
            }
            // Empty last element
            strcpy(entries[writePtr].key,"");
            entries[writePtr].value = 0;

            entries[0].value--;
            writePtr--;
            
            return true;
        }
    }
    return false;
}

int UnorderedMapShm::destroy() {
    // Implement later
    return 0; // Return status
}

bool UnorderedMapShm::find(char *key) {
    bool res;

    return res;
}

