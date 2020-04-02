#ifndef SIMPLEDU_STAT_H_INCLUDED
#define SIMPLEDU_STAT_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

/**
 * @brief Get size of file, in number of blocks.
 * 
 * @param path          Path of file
 * @param apparent_size Apparent or real size
 * @return off_t        Size in bytes if successful, -1 if failed
 */
off_t simpledu_stat(const char *path, bool apparent_size);

off_t simpledu_block(off_t size, off_t blocksize);

/**
 * @brief Mode of a file
 */
typedef enum {
    simpledu_mode_reg,
    simpledu_mode_dir,
    simpledu_mode_lnk,
    simpledu_mode_non
} simpledu_mode_t;

/**
 * @brief Get type/mode of file in path
 * 
 * @param path  Path of file
 * @param m     To store mode
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int simpledu_mode(const char *path, simpledu_mode_t *m);

/**
 * @brief Gives the number of bytes of a file.
 * 
 * @param path      Path of file
 * @return int      File Size in bytes; -1 in case of error
 */
off_t simpledu_file_size(const char *path);

#endif //SIMPLEDU_STAT_H_INCLUDED
