#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>
#include <signal.h>

int main()
{
    int sensorPID, comPID, logPID = 0;
    std::cout << "Hello, sensor! " << getpid() << std::endl;
    // MPSharedMemoryObject::remove(MINI_PROJECT_SHM_NAME);
    MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_RDWR, 0666);
    std::cout <<"Sensor process is running" <<std::endl;
    UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, PROT_READ | PROT_WRITE);
    if (mapShm.read((char*)"SENSOR_PID", &sensorPID))
    {
      std::cout << "SENSOR_PID = " << sensorPID << std::endl;
    }
    if (mapShm.read((char*)"COMMUNICATION_PID", &comPID));
    {
      std::cout << "COMMUNICATION_PID  = " << comPID << std::endl;
    }
    if (mapShm.read((char*)"LOG_PID", &logPID))
    {
      std::cout << "LOG_PID = " << logPID << std::endl;
    }
    // Kill the process with the specified PID
    if (kill(sensorPID, SIGKILL) == 0)
    {
        std::cout << "Successfully killed the process with PID " << sensorPID << std::endl;
    }
    else
    {
        perror("Failed to kill the process");
    }
    if (kill(logPID, SIGKILL) == 0)
    {
        std::cout << "Successfully killed the process with PID " << logPID << std::endl;
    }
    else
    {
        perror("Failed to kill the process");
    }
    if (kill(comPID, SIGKILL) == 0)
    {
        std::cout << "Successfully killed the process with PID " << comPID << std::endl;
    }
    else
    {
        perror("Failed to kill the process");
    }
  return 0;
}