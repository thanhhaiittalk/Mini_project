#ifndef SETUP_HPP
#define SETUP_HPP

#define MINI_PROJECT_SHM_NAME "/mini_project_shared_memory"
#define MINI_PROJECT_SHM_SIZE (4096)
#define MINI_PROJECT_COMMUNICATION_PROGRAM_PATH "./communication"
#define MINI_PROJECT_LOG_PROGRAM_PATH "./log"
#define MINI_PROJECT_SENSOR_PROGRAM_PATH "./sensor"

#define MP_COM_PID "COMMUNICATION_PID"
#define MP_SENSOR_PID "SENSOR_PID"
#define MP_LOG_PID "LOG_PID"

#define MP_SENSOR_FIFO_PATH "/tmp/sensor_fifo"
#define MP_COM_FIFO_PATH "/tmp/com_fifo"
#define MP_LOG_FIFO_PATH "/tmp/log_fifo"

#endif // SETUP_HPP