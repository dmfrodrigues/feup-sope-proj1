#include "simpledu_args.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

const simpledu_args_t simpledu_args_default = {false, 1024, false, false, false, PATH_MAX/2, 0, NULL};

const char optstring[] = "ablLSBd";

int simpledu_args_ctor(simpledu_args_t *p, int argc, const char * const argv[]){
    *p = simpledu_args_default;
    int block_size, max_depth;
    for(int i = 1; i < argc; ++i){
        const char *s = argv[i];
        if(strcmp(s, "-a") == 0 || strcmp(s, "--all"          ) == 0){ p->all           = true; continue; }
        if(strcmp(s, "-b") == 0 || strcmp(s, "--bytes"        ) == 0){ p->block_size    =    1; continue; }
        if(strcmp(s, "-l") == 0 || strcmp(s, "--count-links"  ) == 0){ p->count_links   = true; continue; }
        if(strcmp(s, "-L") == 0 || strcmp(s, "--dereference"  ) == 0){ p->dereference   = true; continue; }
        if(strcmp(s, "-S") == 0 || strcmp(s, "--separate-dirs") == 0){ p->separate_dirs = true; continue; }
        if(strcmp(s, "-B") == 0){
            s = argv[++i];
            if(sscanf(s, "%llu", &p->block_size) != 1){
                errno = EINVAL;
                return EXIT_FAILURE;
            }
            continue;
        }
        if(sscanf(s, "--blocksize=%d", &block_size) == 1){ p->block_size = block_size; continue; }
        if(strcmp(s, "-d") == 0){
            s = argv[++i];
            if(sscanf(s, "%hu", &p->max_depth) != 1){
                errno = EINVAL;
                return EXIT_FAILURE;
            }
            continue;
        }
        if(sscanf(s, "--max-depth=%d", &max_depth) == 1){ p->max_depth = max_depth; continue; }
        char *c = malloc(sizeof(char)*(strlen(s)+1));
        strcpy(c, s);
        ++p->filesc;
        p->files = realloc(p->files, sizeof(char*)*p->filesc);
        p->files[p->filesc-1] = c;
    }
    if(p->filesc == 0){
        char *c = ".";
        ++p->filesc;
        p->files = realloc(p->files, sizeof(char*)*p->filesc);
        p->files[p->filesc-1] = c;
    }
    return EXIT_SUCCESS;
}

int simpledu_args_dtor(simpledu_args_t *p){
    for(int i = 0; i < p->filesc; ++i) free(p->files[i]);
    free(p->files);
    return EXIT_SUCCESS;
}

bool simpledu_args_equal(const simpledu_args_t *p1, const simpledu_args_t *p2){
    if(p1 == p2) return true;
    if(p1 == NULL || p2 == NULL) return false;
    if(!(
        p1->all             == p2->all              &&
        p1->block_size      == p2->block_size       &&
        p1->count_links     == p2->count_links      &&
        p1->dereference     == p2->dereference      &&
        p1->separate_dirs   == p2->separate_dirs    &&
        p1->max_depth       == p2->max_depth        && 
        p1->filesc          == p2->filesc
    )) return false;
    for(size_t i = 0; i < p1->filesc; ++i){
        if(strcmp(p1->files[i], p2->files[i]) != 0) return false;
    }
    return true;
}
