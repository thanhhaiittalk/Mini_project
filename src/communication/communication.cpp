#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>           
#include <cstring>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"

using namespace std;
int main() {
    cout<<"Communication "<< getpid() <<endl;
    MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);
    UnorderedMapShm mapShm(shmObj,MINI_PROJECT_SHM_SIZE,O_CREAT | O_RDWR);
    mapShm.write((char*)"COMMUNICATION_PID",getpid());
    while (1) {
        sleep(2);
    }
    return 0;
}