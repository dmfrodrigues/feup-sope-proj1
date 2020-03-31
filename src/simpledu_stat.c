#include "simpledu_stat.h"
#include "simpledu_args.h"

#include <stdlib.h>

#include <stdio.h>

off_t simpledu_stat(const char *path, bool apparent_size, off_t blocksize){
    struct stat buf;
    if(stat(path, &buf))
        return -1;
    printf("path: %s\n", path);
    printf("st_size: %ld\nst_blksize: %ld\nst_blocks: %ld\n", buf.st_size, buf.st_blksize, buf.st_blocks);

    if (apparent_size) return buf.st_size/blocksize;

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

