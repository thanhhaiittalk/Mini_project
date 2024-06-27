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

void executeProgram(const char *program, char *const args[]);
void setupSharedMemory();
void setupProgram();
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

    // Write into shared memory
    UnorderedMapShm mapShm(shmObj,MINI_PROJECT_SHM_SIZE,O_CREAT | O_RDWR);
    mapShm.write((char*)"TEST_SETUP",100);
    mapShm.write((char*)"TEST_SETUP_1",101);
    mapShm.write((char*)"TEST_SETUP_2",102);

    // Read
    int testValue, testValue_1, testValue_2 = 0; 
    mapShm.read((char*)"TEST_SETUP", &testValue);
    std::cout<<"Test setup = "<<  testValue << std::endl;
    mapShm.read((char*)"TEST_SETUP_1", &testValue_1);
    std::cout<<"Test setup 1 = "<<  testValue_1 << std::endl;
    mapShm.read((char*)"TEST_SETUP_2", &testValue_2);
    std::cout<<"Test setup 2 = "<<  testValue_2 << std::endl;

    mapShm.write((char*)"TEST_SETUP_2",1002);
    mapShm.read((char*)"TEST_SETUP_2", &testValue_2);

    mapShm.remove((char*)"TEST_SETUP_2");
    if (!mapShm.read((char*)"TEST_SETUP_2",&testValue_2)) {
        std::cout << "Not found TEST_SETUP_2" << std::endl;
    }
    std::cout<<"Update Test setup 2 = "<<  testValue_2 << std::endl;
}

void setupProgram()
{
    const char *communcation_program = MINI_PROJECT_COMMUNICATION_PROGRAM_PATH;
    const char *log_program = MINI_PROJECT_LOG_PROGRAM_PATH;
    const char *sensor_program = MINI_PROJECT_SENSOR_PROGRAM_PATH;
    //* Execute programs*//
    // Execute the communication program
    executeProgram(communcation_program, NULL);
    
    // Execute the log program
    executeProgram(log_program, NULL);

    // Execute the sensor program
    executeProgram(sensor_program, NULL);
}

void setupMessageQueue()
{

}

int main() {
    std::cout<<"This is setup"<<std::endl;
    // Setup Shared memory
    setupSharedMemory();

    // Setup Message queue
    setupMessageQueue();

    // Setup Program
    setupProgram();

    return 0;
}
