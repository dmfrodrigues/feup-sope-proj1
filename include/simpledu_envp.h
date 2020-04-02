#ifndef SIMPLEDU_ENVP_H_INCLUDED
#define SIMPLEDU_ENVP_H_INCLUDED

/**
 * @brief Holds all environment variables that 'simpledu' requires.
 */
typedef struct{
    /**
     * @brief Filename of log.
     */
    char *LOG_FILENAME;
} simpledu_envp_t;

/**
 * @brief simpledu_args constructor.
 * 
 * @param p     Pointer to simpledu_envp_t to construct
 * @param envp  Environment variables, NULL-terminated array of strings
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_envp_ctor(simpledu_envp_t *p, char *envp[]);

/**
 * @brief simpledu_envp destructor.
 * 
 * @param p     Pointer to simpledu_envp_t to destruct
 * @return int  EXIT_SUCCESS if successful
 */
int simpledu_envp_dtor(simpledu_envp_t *p);

#endif //SIMPLEDU_ENVP_H_INCLUDED
