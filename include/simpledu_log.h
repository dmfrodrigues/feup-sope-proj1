#ifndef SIMPLEDU_LOG_H_INCLUDED
#define SIMPLEDU_LOG_H_INCLUDED

#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Create logfile.
 * 
 * @param filename  Filename
 * @return int      Same as open(2)
 */
int simpledu_log_createlog(const char * filename);

/**
 * @brief Log a CREATE action.
 * 
 * Should be used right after entering main.
 * 
 * @param argc  Number of arguments
 * @param argv  Arguments vector
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_log_CREATE(int argc, const char* const argv[]);

/**
 * @brief Log a RECV_SIGNAL action.
 * 
 * Should be used by the process receiving the signal, right after receiving the signal.
 * 
 * @param sig   Signal received
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_log_RECV_SIGNAL(int sig);

/**
 * @brief Log an ENTRY action.
 * 
 * Should be used just before writting a line in stdout.
 * 
 * @param size      Size of folder/file
 * @param path      Path of folder/file being logged
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_log_ENTRY(long long unsigned size, const char *path);

/**
 * @brief Cause normal process termination.
 * 
 * Must be used instead of exit(3) to allow logging.
 * 
 * @param status    Exit status
 */
void simpledu_exit(int status);

/**
 * @brief Send signal to process.
 * 
 * Must be used instead of kill(2) to allow logging.
 * 
 * @param pid   Process ID to send the signal to
 * @param sig   Signal to send
 * @return int  EXIT_SUCCESS if successful, otherwise EXIT_FAILURE and errno is set
 */
int simpledu_kill(pid_t pid, int sig);

#endif //SIMPLEDU_LOG_H_INCLUDED