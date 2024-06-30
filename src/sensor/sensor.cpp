#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>

int main()
{
  int sensorPID, comPID, logPID = 0;
  std::cout << "Hello, sensor! " << getpid() << std::endl;
  // MPSharedMemoryObject::remove(MINI_PROJECT_SHM_NAME);
  MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_RDWR, 0666);
  // // Configure size of the shared memory object
  // int fd = shm_open (MINI_PROJECT_SHM_NAME, O_RDWR, 0666);
  // if (fd == -1) {
  //   std::cerr << "Failed to open " << MINI_PROJECT_SHM_NAME << std::endl;
  // }
  // void* ptr = mmap(0, MINI_PROJECT_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  // if (ptr == MAP_FAILED) {
  //       std::cerr << "Failed to map shared memory" << std::endl;
  // }
  // std::cout << "Sensor: Before read entries" << *ptr <<std::endl;
  
  // UnorderedMapShm::HashEntryType *entries = static_cast<UnorderedMapShm::HashEntryType *>(ptr);
  // std::cout << "Sensor: entris[0].key = " << entries[0].key << std::endl; 
  UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, PROT_READ | PROT_WRITE);
  mapShm.write((char*)"SENSOR_PID", getpid());
  // int sensorPID, comPID, logPID = 0;
  while (1)
  {
    std::cout <<"Sensor process is running" <<std::endl;
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
    mapShm.write((char*)"TEST_SENSOR",1234);
    sleep(2);
  }
  return 0;
}