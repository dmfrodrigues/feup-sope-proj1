#include "simpledu_stat.h"
#include "simpledu_args.h"

#include <stdlib.h>

#include <stdio.h>

static const off_t stat_std_block_size = 512;

off_t simpledu_stat(const char *path, bool apparent_size, off_t blocksize){
    struct stat buf;
    if(lstat(path, &buf)) return -1;
    off_t result = (apparent_size ? buf.st_size : buf.st_blocks*stat_std_block_size);
    return result/blocksize + (result%blocksize ? 1 : 0);
}

bool simpledu_symb_link(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return -1;
    return S_ISLNK(buf.st_mode);
}

bool simpledu_dir(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return 0;
    return S_ISDIR(buf.st_mode);
}

bool simpledu_reg_file(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return 0;
    return S_ISREG(buf.st_mode);
}

off_t simpledu_file_size(const char *path) {
    struct stat buf;
    if(lstat(path, &buf)) return -1;
    return buf.st_size;
}

