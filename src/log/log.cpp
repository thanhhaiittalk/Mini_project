#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  int test = 0;
  std::cout << "Hello, log! " << getpid() << std::endl;
  MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);
  UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, O_CREAT | O_RDWR);
  mapShm.write((char*)"LOG_PID", getpid());

  // Write to FIFO PID to inform completed write PID in shared memory
  mkfifo((const char *)MP_LOG_FIFO_PATH, 0666);
  int fd = open((const char *)MP_LOG_FIFO_PATH, 0666);
  if (fd == -1)
  {
    std::cerr << "Failed to open FIFO for writing" << std::endl;
    close(fd);
    return 1;
  }
  const int logPID = getpid();
  std::cout << "log: Write log PID " << logPID << " to FIFO." << std::endl;
  write(fd, &logPID, sizeof(logPID));
  close(fd);
  while (1)
  {
    sleep(2);
  }
  return 0;
}