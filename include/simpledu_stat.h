#ifndef SIMPLEDU_STAT_H_INCLUDED
#define SIMPLEDU_STAT_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Get size of file, in number of blocks.
 * 
 * @param path      Path of file
 * @param blocksize Block size in bytes
 * @return off_t    Number of blocks if successful, -1 if failed
 */
off_t simpledu_stat(const char *path, off_t blocksize);

#endif //SIMPLEDU_STAT_H_INCLUDED
