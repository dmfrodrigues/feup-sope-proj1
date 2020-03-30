#include "simpledu_stat.h"

#include <stdlib.h>

off_t simpledu_stat(const char *path, off_t blocksize){
    struct stat buf;
    if(stat(path, &buf)) return -1;
    int result = buf.st_blocks * 512;
    if (result % blocksize != 0) return (result / blocksize) + 1;
    else return result / blocksize;
}


int simpledu_symb_link(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return -1;
    return S_ISLNK(buf.st_mode);
}

int simpledu_dir(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return 0;
    return S_ISDIR(buf.st_mode);
}

int simpledu_reg_file(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return 0;
    return S_ISREG(buf.st_mode);
}

off_t simpledu_file_size(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return -1;
    return buf.st_size;
}

