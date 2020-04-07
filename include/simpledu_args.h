#ifndef SIMPLEDU_ARGS_H_INCLUDED
#define SIMPLEDU_ARGS_H_INCLUDED

#include "simpledu_time.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdarg.h>

/**
 * @brief Holds all argument options with which 'simpledu' was called.
 */
typedef struct {
    /**
     * -a, --all
     * write counts for all files, not just directories
     */
    bool all;
    /**
     * --apparent-size
     * print apparent sizes, rather than disk usage;
     * This option is not available for shell use, given it is only supposed to be activated by -b
     */
    bool apparent_size;
    /**
     * -b, --bytes
     *  write real size in bytes
     */
    /**
     * -B, --block-size=SIZE
     * scale sizes by SIZE before printing them; defaults to 1024
     */
    long long unsigned block_size;
    /**
     * -l, --count-links
     * count sizes many times if hard linked
     */
    bool count_links;
    /**
     * -L, --dereference
     * dereference all symbolic links; 'follows' symbolic links
     */
    bool dereference;
    /**
     * -S, --separate-dirs
     * for directories do not include size of subdirectories
     */
    bool separate_dirs;
    /**
     * -d, --max-depth=N
     * print the total for a directory (or file with --all) only if it is N or fewer levels below the command line argument
     */
    short max_depth;
    /**
     * -f, --log-filedes
     * set file descriptor of log file; should only be used by subprocesses
     */
    int log_filedes;
    /**
     * -p, --pipe-filedes
     * set file descriptor to pipe to communicate with parent; should only be used by subprocesses
     */
    int pipe_filedes;

    micro_t start_time;

    pid_t children_process_group;
    /**
     * @brief Size of files array.
     */
    size_t filesc;
    /**
     * @brief Files to analyse.
     */
    char **files;
} simpledu_args_t;

/**
 * @brief simpledu_args constructor.
 * 
 * @param p     Pointer to simpledu_args_t to construct
 * @param argc  Size of argv
 * @param argv  Arguments passed to program, to fill simpledu_args_t
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]);

/**
 * @brief simpledu_args destructor.
 * 
 * @param p     Pointer to simpledu_args_t to destruct
 * @return int  EXIT_SUCCESS if successful
 */
int simpledu_args_dtor(simpledu_args_t *p);

/**
 * @brief Compare two simpledu_args_t instances for equality.
 * 
 * @param p1        First argument
 * @param p2        Second argument
 * @return true     If they are equal
 * @return false    Otherwise
 */
bool simpledu_args_equal(const simpledu_args_t *p1, const simpledu_args_t *p2);

/**
 * @brief Deep copy contents from a simpledu_args to another.
 * 
 * @param p1        Destination
 * @param p2        Source
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_args_copy(simpledu_args_t *p1, const simpledu_args_t *p2);

/**
 * @brief Set list of files.
 * 
 * This function has a variable number of arguments.
 * 
 * @param p     simpledu_args to change
 * @param num   Number of arguments after this one
 * @param ...   List of char pointers
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_args_set_files(simpledu_args_t *p, int num,...);

/**
 * @brief Convert simpledu_args to a representation equivalent to an argv.
 * 
 * @param p     Source
 * @param pargv Pointer to future argv
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_args_toargv(const simpledu_args_t *p, char ***pargv);

simpledu_args_t arg;

#endif //SIMPLEDU_ARGS_H_INCLUDED
