#include <iostream>
#include <unistd.h>
#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);
UnorderedMapShm mapShm(shmObj, MINI_PROJECT_SHM_SIZE, O_CREAT | O_RDWR);
pthread_mutex_t mutex;  // Declare the mutex
FILE *file;
// Thread function
void *thread_log(void *arg)
{
  printf("Thread is logging...\n");
  int sensor = *((int*)arg);
  pthread_mutex_lock(&mutex);
  fprintf(file, "Sensor data: %d!\r\n",sensor);
  // Flush the file's output buffer to the disk
  if (fflush(file) != 0)
  {
    perror("fflush");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  pthread_mutex_unlock(&mutex);
  return NULL;
}

// Signal handler function
void handle_sigusr1(int sig)
{
  int sensorData;
  std::cout << "Received SIGUSR1 signal\n";
  mapShm.read("SENSOR_DATA", &sensorData);
  std::cout << "LOG: SENSOR_DATA = " << sensorData << std::endl;

  /*Create a thread to log data into file*/
  // Create a thread
  pthread_t thread;
  if (pthread_create(&thread, NULL, thread_log, (void*)&sensorData) != 0)
  {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }

  // Detach the thread so that it releases resources automatically upon termination
  if (pthread_detach(thread) != 0)
  {
    perror("pthread_detach");
    exit(EXIT_FAILURE);
  }
}

int main()
{
  // Open a file for writing (text mode)
  file = fopen("mini_project_log.txt", "w");
  if (file == NULL)
  {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // Initialize the mutex
  if (pthread_mutex_init(&mutex, NULL) != 0)
  {
    perror("pthread_mutex_init");
    return 1;
  }

  std::cout << "Hello, log! " << getpid() << std::endl;

  mapShm.write((char *)"LOG_PID", getpid());

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
  while (1)
  {
    sleep(2);
  }
  return 0;
}