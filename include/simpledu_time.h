#ifndef SIMPLEDU_TIME_H_INCLUDED
#define SIMPLEDU_TIME_H_INCLUDED

typedef long long micro_t;

/**
 * @brief Start time counting.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_starttime(micro_t *micros_since_epoch);

/**
 * @brief Set starting point in time.
 * 
 * @param micros_since_epoch    Number of microseconds since the epoch
 * @return int                  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_set_starttime(micro_t micros_since_epoch);

/**
 * @brief Get number of milliseconds since time was started.
 * 
 * @param d     Pointer to double where time will be stored
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_gettime(double *d);

#endif //SIMPLEDU_TIME_H_INCLUDED
