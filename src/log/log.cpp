#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>

int main()
{
  std::cout << "Hello, log! " << getpid() << std::endl;
  MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);
  UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, O_CREAT | O_RDWR);
  mapShm.write((char*)"LOG_PID", getpid());
  while (1)
  {
    sleep(2);
  }
  return 0;
}