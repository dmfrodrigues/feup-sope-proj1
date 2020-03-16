#ifndef SIMPLEDU_ARGS_H
#define SIMPLEDU_ARGS_H

#include <linux/limits.h>
#include <stdbool.h>

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
    bool bytes;
    /**
     * -B, --blocksize=SIZE
     * scale sizes by SIZE before printing them; defaults to 1024
     */
    int  block_size;
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
    int  max_depth;
} simpledu_args_t;

/**
 * @brief Default arguments.
 */
const simpledu_args_t simpledu_args_default;

/**
 * @brief simpledu_args constructor.
 */
int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]);

#endif //SIMPLEDU_ARGS_H
