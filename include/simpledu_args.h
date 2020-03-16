#ifndef SIMPLEDU_ARGS_H
#define SIMPLEDU_ARGS_H

#include <stdlib.h>
#include <stdbool.h>
#include <linux/limits.h>

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
     * -b, --bytes
     *  equivalent to '--block-size=1'; write real size in bytes
     */
    /**
     * -B, --blocksize=SIZE
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
    unsigned short max_depth;
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
int simpledu_args_ctor(simpledu_args_t *p, int argc, const char * const argv[]);

/**
 * @brief simpledu_args destructor.
 * 
 * @param p     Pointer to simpledu_args_t to destruct
 * @return int  EXIT_SUCCESS if successful
 */
int simpledu_args_dtor(simpledu_args_t *p);

/**
 * @brief Compare two simpledu_args_t instances of equality.
 * 
 * @param p1        First argument
 * @param p2        Second argument
 * @return true     If they are equal
 * @return false    Otherwise
 */
bool simpledu_args_equal(const simpledu_args_t *p1, const simpledu_args_t *p2);

#endif //SIMPLEDU_ARGS_H
