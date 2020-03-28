#include "simpledu_stat.h"

#include <stdlib.h>

off_t simpledu_stat(const char *path, off_t blocksize){
    struct stat buf;
    if(stat(path, &buf)) return -1;
    return buf.st_size/blocksize;
}


bool simpledu_symb_link(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return -1;
    return S_ISLNK(buf.st_mode);
}


bool simpledu_dir(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return -1;
    return S_ISDIR(buf.st_mode);
}

int simpledu_reg_file(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return 0;
    return S_ISREG(buf.st_mode);
}

long long simpledu_file_size(const char *path) {
    struct stat buf;
    if(stat(path, &buf)) return -1;
    return S_ISREG(buf.st_size);
}

