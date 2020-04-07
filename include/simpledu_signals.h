#ifndef SIMPLEDU_SIGNALS_H_INCLUDED
#define SIMPLEDU_SIGNALS_H_INCLUDED

/**
 * @brief Handles installing of signal handler and flags
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_handler();

/**
 * @brief SIGINT handler for main process
 * 
 * @param signo 
 */
void sigint_handler(int signo);

#endif //SIMPLEDU_SIGNALS_H_INCLUDED