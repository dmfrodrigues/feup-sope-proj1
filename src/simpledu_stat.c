#include "simpledu_stat.h"
#include "simpledu_args.h"

#include <stdlib.h>

#include <stdio.h>

static const off_t stat_std_block_size = 512;

off_t simpledu_stat(const char *path, bool apparent_size){
    struct stat buf;

    if(lstat(path, &buf)) return -1;
    return (apparent_size ? buf.st_size : buf.st_blocks*stat_std_block_size);
}

off_t simpledu_block(off_t size, off_t blocksize){
    return size/blocksize + (size%blocksize ? 1 : 0);
}

int simpledu_mode(const char *path, simpledu_mode_t *m){
    struct stat buf;
    if(lstat(path, &buf)) return EXIT_FAILURE;
    if     (S_ISREG(buf.st_mode)) *m = simpledu_mode_reg;
    else if(S_ISDIR(buf.st_mode)) *m = simpledu_mode_dir;
    else if(S_ISLNK(buf.st_mode)) *m = simpledu_mode_lnk;
    else                          *m = simpledu_mode_non;
    return EXIT_SUCCESS;
}
