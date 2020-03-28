#ifndef SIMPLEDU_STAT_H_INCLUDED
#define SIMPLEDU_STAT_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * @brief Get size of file, in number of blocks.
 * 
 * @param path      Path of file
 * @param blocksize Block size in bytes
 * @return off_t    Number of blocks if successful, -1 if failed
 */
off_t simpledu_stat(const char *path, off_t blocksize);


/**
 * @brief Check if a file is a symbolic link or not.
 * 
 * @param path      Path of file
 * @return true     If symbolic link
 * @return false    Otherwise
 */
bool simpledu_symb_link(const char *path);

/**
 * @brief Check if a file is a directory or not.
 * 
 * @param path      Path of file
 * @return true     If directory
 * @return false    Otherwise
 */
bool simpledu_dir(const char *path);

/**
 * @brief Check if a file is a regular file or not.
 * 
 * @param path      Path of file
 * @return true     If regular file
 * @return false    Otherwise
 */
bool simpledu_reg_file(const char *path);


/**
 * @brief Gives the number of bytes of a file.
 * 
 * @param path      Path of file
 * @return int      File Size in bytes; -1 in case of error
 */
long long simpledu_file_size(const char *path);

#endif //SIMPLEDU_STAT_H_INCLUDED
