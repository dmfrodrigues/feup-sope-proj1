#ifndef SIMPLEDU_TIME_H_INCLUDED
#define SIMPLEDU_TIME_H_INCLUDED

/**
 * @brief Start time counting.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_starttime();

/**
 * @brief Get number of milliseconds since time was started.
 * 
 * @param d     Pointer to double where time will be stored
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_gettime(double *d);

#endif //SIMPLEDU_TIME_H_INCLUDED
