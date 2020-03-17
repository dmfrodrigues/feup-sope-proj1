#include "simpledu_args.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

const simpledu_args_t simpledu_args_default = {false, 1024, false, false, false, PATH_MAX/2, 0, NULL};

const char optstring[] = "abLlSB:d:";
const struct option longopts[] = {
    {"all"          , 0, NULL, 'a'},
    {"bytes"        , 0, NULL, 'b'},
    {"dereference"  , 0, NULL, 'L'},
    {"count-links"  , 0, NULL, 'l'},
    {"separate-dirs", 0, NULL, 'S'},
    {"blocksize"    , 1, NULL, 'B'},
    {"max-depth"    , 1, NULL, 'd'},
    {0,0,0,0}
};

int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]){
    *p = simpledu_args_default;

    opterr = 0; optind = 1;
    int opt = 0; int longindex;
    while((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1){
        switch(opt){
            case 'a': p->all           = true; break;
            case 'b': p->block_size    =    1; break;
            case 'L': p->dereference   = true; break;
            case 'l': p->count_links   = true; break;
            case 'S': p->separate_dirs = true; break;
            case 'B': if(sscanf(optarg, "%llu", &p->block_size) != 1) return EXIT_FAILURE; break;
            case 'd': if(sscanf(optarg, "%hu" , &p->max_depth ) != 1) return EXIT_FAILURE; break;
            default: opterr = 1; optind = 1; return EXIT_FAILURE;
        }
    }
    opterr = 1;

    p->filesc = argc - optind;
    p->files  = realloc(p->files, p->filesc*sizeof(char *));
    for(int i = 0; i < p->filesc; ++i){
        const char *s = argv[optind+i];
        char *str = malloc((1+strlen(s)*sizeof(char)));
        strcpy(str, s);
        p->files[i] = str;
    }
    optind = 1;

    if(p->filesc == 0){
        p->filesc = 1;
        p->files  = realloc(p->files, p->filesc*sizeof(char *));
        const char *s = ".";
        char *str = malloc(2*sizeof(char));
        strcpy(str, s);
        p->files[0] = str;
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
