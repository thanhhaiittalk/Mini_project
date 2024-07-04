#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>           
#include <unistd.h>
#include <cstring>
#include <sys/mman.h>

#include "setup.hpp"
#include "mp_shared_memory_object.hpp"
#include "unordered_map_shm.hpp"

// Public variable
int sensorPID, comPID, logPID;

void executeProgram(const char *program, char *const args[]);
void setupSharedMemory();
void setupProgram();

void waitSetupComplete();
void waitFIFO(const char* path, int* data);
void setupMessageQueue();

void executeProgram(const char *program, char *const args[]) {
    pid_t pid = fork();
    
    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    } else if (pid == 0) {
        // In child process
        if (execv(program, args) < 0) {
            std::cerr << "Exec failed" << std::endl;
            exit(1);
        }
    } else {
        // In parent process, wait for the child to complete
        // int status;
        // waitpid(pid, &status, 0);
    }
}

void setupSharedMemory()
{
    
    // Remove any same name shared memory
    MPSharedMemoryObject::remove(MINI_PROJECT_SHM_NAME);

    // Create shared memory object
    MPSharedMemoryObject shmObj(MINI_PROJECT_SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Configure size of the shared memory object
    shmObj.truncate(MINI_PROJECT_SHM_SIZE);
}

void setupProgram()
{
    const char *communcation_program = MINI_PROJECT_COMMUNICATION_PROGRAM_PATH;
    const char *log_program = MINI_PROJECT_LOG_PROGRAM_PATH;
    const char *sensor_program = MINI_PROJECT_SENSOR_PROGRAM_PATH;
    //* Execute programs*//
    // Execute the communication program
    executeProgram(communcation_program, NULL);
    // Wait com FIFO
    waitFIFO(MP_COM_FIFO_PATH, &comPID);
    
    // // Execute the log program
    executeProgram(log_program, NULL);
    // Wait log FIFO
    waitFIFO(MP_LOG_FIFO_PATH, &logPID);

    // Execute the sensor program
    executeProgram(sensor_program, NULL);
    // Wait sensor FIFO
    waitFIFO(MP_SENSOR_FIFO_PATH,&sensorPID);

    // Send signal to sensor process, notify other processes complete write PID to shared memory
    // Send SIGUSR1 to the target process
    if (kill(sensorPID, SIGUSR1) == -1) {
        perror("Failed to send signal");
        return;
    } 

    printf("Sent SIGUSR1 to process %d\n", sensorPID);
}

void setupMessageQueue()
{

}

void waitFIFO(const char* path, int* data)
{
    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        std::cerr << "Failed to open FIFO for reading: " << strerror(errno) << std::endl;
        return;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 10;  // Wait for 5 seconds
    timeout.tv_usec = 0;

    std::cout << "Waiting for data in FIFO for up to 5 seconds..."<<path << std::endl;
    int retval = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if (retval == -1) {
        std::cerr << "select() failed: " << strerror(errno) << std::endl;
        close(fd);
        return;
    } else if (retval == 0) {
        std::cout << "No data within the timeout period."<< path << std::endl;
    } else {
        if (FD_ISSET(fd, &readfds)) {
            // std::cout << "Data is available in FIFO." << std::endl;
            ssize_t bytesRead = read(fd, data, sizeof(*data));
            if (bytesRead == sizeof(*data)) {
                std::cout << "Read PID "<< path <<" "<< *data << " from FIFO." << std::endl;
            } else {
                std::cerr << "Failed to read from FIFO: " << strerror(errno) << std::endl;
            }
        }
    }

    close(fd);

}

void waitSetupComplete() 
{
    // Wait com FIFO
    waitFIFO(MP_COM_FIFO_PATH, &comPID);

    // Wait log FIFO
    waitFIFO(MP_LOG_FIFO_PATH, &logPID);

    // Wait sensor FIFO
    waitFIFO(MP_SENSOR_FIFO_PATH,&sensorPID);
    

    // Send signal to sensor process, notify other processes complete write PID to shared memory
    // Send SIGUSR1 to the target process
    printf("Sent SIGUSR1 to process %d\n", sensorPID);
    if (kill(sensorPID, SIGUSR1) == -1) {
        perror("Failed to send signal");
        return;
    } 

}

int main() {
    std::cout<<"This is setup"<<std::endl;
    // Setup Shared memory
    setupSharedMemory();

    // Setup Message queue
    setupMessageQueue();

    // Setup Program
    setupProgram();

    // Wait other programs of project completed
    // waitSetupComplete();

    std::cout <<"Complete setup"<<std::endl;
    return 0;
}
