#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>           
#include <cstring>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include "setup.hpp"
#include <signal.h>
#include <unistd.h>

using namespace std;

MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);
UnorderedMapShm mapShm(shmObj,MINI_PROJECT_SHM_SIZE,O_CREAT | O_RDWR);

// Signal handler function
void handle_sigusr1(int sig) {
  int sensorData;
  std::cout << "Received SIGUSR1 signal\n";
  mapShm.read("SENSOR_DATA", &sensorData);
  std::cout << "COMMUNICATION: SENSOR_DATA = " << sensorData << std::endl;
}

int main() {
    cout<<"Communication "<< getpid() <<endl;

    mapShm.write((char*)"COMMUNICATION_PID",getpid());

    // Write to FIFO PID to inform completed write PID in shared memory
    mkfifo((const char *)MP_COM_FIFO_PATH, 0666);
    int fd = open((const char *)MP_COM_FIFO_PATH, 0666);
    if (fd == -1)
    {
        std::cerr << "Failed to open FIFO for writing" << std::endl;
        close(fd);
        return 1;
    }
    const int comPID = getpid();
    std::cout << "communication: Write com PID " << comPID << " to FIFO." << std::endl;
    write(fd, &comPID, sizeof(comPID));
    close(fd);

    struct sigaction sa;

    // Set up the sigaction structure to specify the signal handler
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = 0;          // No special flags
    sigemptyset(&sa.sa_mask); // Block no signals during the execution of the handler

    // Register the signal handler for SIGUSR1
    if (sigaction(SIGUSR1, &sa, nullptr) == -1)
    {
        std::cerr << "Unable to catch SIGUSR1\n";
        return EXIT_FAILURE;
    }
    while (1) {
        // cout<<"Communication is running "<< getpid() <<endl;
        sleep(2);
    }
    return 0;
}