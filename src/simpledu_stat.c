#include "simpledu_stat.h"

#include <stdlib.h>

off_t simpledu_stat(const char *path, off_t blocksize){
    struct stat buf;
    if(stat(path, &buf)) return -1;
    return buf.st_size/blocksize;
}
