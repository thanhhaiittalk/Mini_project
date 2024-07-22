#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>
#include <signal.h>
#include <setup.hpp>
#include <sys/stat.h>

int main()
{
  int comPID, logPID = 0;
  bool comExist, logExist = false;
  int sensorData = 0;

  sigset_t set;
  int sig;
  if (sigemptyset(&set) == -1) {
      std::cerr<<("sigemptyset")<<std::endl;
      exit(EXIT_FAILURE);
  }

  if (sigaddset(&set, SIGUSR1) == -1)
  {
    std::cerr << ("sigaddset") << std::endl;
    exit(EXIT_FAILURE);
  }

  // Block SIGUSR1 signal
  if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
  {
    std::cerr << ("sigprocmask")<<std::endl;
    exit(EXIT_FAILURE);
  }
  
  std::cout << "Hello, sensor! " << getpid() << std::endl;
  // MPSharedMemoryObject::remove(MINI_PROJECT_SHM_NAME);
  MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_RDWR, 0666);
  UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, PROT_READ | PROT_WRITE); 
  mapShm.write((char*)"SENSOR_PID", getpid());

  // Write to FIFO PID to inform completed write PID in shared memory
  mkfifo((const char*)MP_SENSOR_FIFO_PATH, 0666);
  int fd = open((const char*)MP_SENSOR_FIFO_PATH, 0666);
  if (fd == -1) {
    std::cerr << "Failed to open FIFO for writing" << std::endl;
    close(fd);
    return 1;
  }
  const int sensorPID = getpid();
  std::cout << "sensor: Write Sensor PID " << sensorPID << " to FIFO." << std::endl;
  write(fd, &sensorPID, sizeof(sensorPID));
  close(fd);


  // Need to wait until completed
  // Initialize the signal set to be empty
  
  std::cout<<"Sensor Wait signal ..." <<std::endl;
  if (sigwait(&set, &sig) != 0)
  {
    std::cerr<<("sigwait")<<std::endl;
  }
  std::cout<<"Sensor: Wait signal completed" << std::endl;

  comExist = mapShm.read((char*)MP_COM_PID, &comPID);
  logExist = mapShm.read((char*)MP_LOG_PID, &logPID);
  std::cout<<"Sensor: COM PID = " << comPID <<std::endl;
  std::cout<<"Sensor: Log PID = " << logPID <<std::endl;

  while (1)
  {
    sensorData++;
    mapShm.write((char*)"SENSOR_DATA",sensorData);
    std::cout << "Sensor is running" << std::endl;
    std::cout << "Sensor is logging data "<< sensorData << std::endl;
    if (kill(comPID, SIGUSR1) == -1)
    {
      std::cerr << ("Failed to send signal") << std::endl;
    }

    if (kill(logPID, SIGUSR1) == -1) {
      std::cerr << ("Failed to send signal") << std::endl;
    } 
    sleep(2);
  }
  return 0;
}