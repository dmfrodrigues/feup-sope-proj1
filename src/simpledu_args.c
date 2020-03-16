#include "simpledu_args.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

const simpledu_args_t simpledu_args_default = {false, 1024, false, false, false, PATH_MAX/2, 0, NULL};

int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]){
    simpledu_args_t ret = simpledu_args_default;
    int block_size, max_depth;
    for(int i = 0; i < argc; ++i){
        const char *s = argv[i];
        if     (strcmp(s, "-a") == 0 || strcmp(s, "--all"          ) == 0) ret.all           = true;
        else if(strcmp(s, "-b") == 0 || strcmp(s, "--bytes"        ) == 0) ret.block_size    =    1;
        else if(strcmp(s, "-l") == 0 || strcmp(s, "--count-links"  ) == 0) ret.count_links   = true;
        else if(strcmp(s, "-L") == 0 || strcmp(s, "--dereference"  ) == 0) ret.dereference   = true;
        else if(strcmp(s, "-S") == 0 || strcmp(s, "--separate-dirs") == 0) ret.separate_dirs = true;
        else if(strcmp(s, "-B")){
            s = argv[++i];
            if(sscanf(s, "%d", &ret.block_size) != 1){
                errno = EINVAL;
                return EXIT_FAILURE;
            }
        }
        else if(sscanf(s, "--blocksize=%d", &block_size) == 1) ret.block_size = block_size;
        else if(strcmp(s, "-d")){
            s = argv[++i];
            if(sscanf(s, "%d", &ret.max_depth) != 1){
                errno = EINVAL;
                return EXIT_FAILURE;
            }
        }
        else if(scanf(s, "--max-depth=%d", &max_depth) == 1) ret.max_depth = max_depth;
        else{
            char *c = malloc(sizeof(char)*(strlen(s)+1));
            strcpy(c, s);
            ++ret.filesc;
            ret.files = realloc(ret.files, sizeof(char*)*ret.filesc);
            ret.files[ret.filesc-1] = c;
        }
    }
    if(ret.filesc == 0){
        char *c = ".";
        ++ret.filesc;
        ret.files = realloc(ret.files, sizeof(char*)*ret.filesc);
        ret.files[ret.filesc-1] = c;
    }
    *p = ret;
    return EXIT_SUCCESS;
}

int simpledu_args_dtor(simpledu_args_t *p){
    for(int i = 0; i < p->filesc; ++i) free(p->files[i]);
    free(p->files);
    return EXIT_SUCCESS;
}
