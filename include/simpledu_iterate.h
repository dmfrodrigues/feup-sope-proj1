#ifndef SIMPLEDU_ITERATE_H_INCLUDED
#define SIMPLEDU_ITERATE_H_INCLUDED

#include "simpledu_args.h"

/**
 * @brief Iterate over content of a folder.
 * 
 * @param path      Path of folder to iterate over
 * @param arg       Argument structure
 * @param envp      Environment variables
 * @param pipe_id   To store pipe file descriptor to be read to get subdirectories size
 * @param size      To store size of folder contents (not counting subfolders)
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_iterate(const char *path, simpledu_args_t arg, char *envp[], int *pipe_id, off_t *size);

/**
 * @brief Retrieve data from pipe.
 * 
 * @param pipe_filedes  File descriptor of pipe to read from
 * @param size          To store size of retrieved data
 * @return int          EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_retrieve(int pipe_filedes, off_t *size);

/**
 * @brief Print size of folder.
 * 
 * @param path      Folder path
 * @param size      Size of folder
 * @param more_size Size of subfolders
 * @param arg       Arguments structure
 * @return int      EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_print(const char *path, off_t size, off_t more_size, simpledu_args_t arg);

#endif //SIMPLEDU_ITERATE_H_INCLUDED