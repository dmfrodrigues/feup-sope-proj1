#include "simpledu_args.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

static const simpledu_args_t simpledu_args_default = {false, 1024, false, false, false, PATH_MAX/2, -1, -1, 0, NULL};

static const char optstring[] = "abLlSB:d:f:p:";
static const struct option longopts[] = {
    {"all"          , 0, NULL, 'a'},
    {"bytes"        , 0, NULL, 'b'},
    {"dereference"  , 0, NULL, 'L'},
    {"count-links"  , 0, NULL, 'l'},
    {"separate-dirs", 0, NULL, 'S'},
    {"blocksize"    , 1, NULL, 'B'},
    {"max-depth"    , 1, NULL, 'd'},
    {"log-filedes"  , 1, NULL, 'f'},
    {"pipe-filedes" , 1, NULL, 'p'},
    {0,0,0,0}
};

int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]){
    if(p == NULL){ errno = EINVAL; return EXIT_FAILURE; }
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
            case 'B': if(sscanf(optarg, "%llu", &p->block_size  ) != 1) return EXIT_FAILURE; break;
            case 'd': if(sscanf(optarg, "%hu" , &p->max_depth   ) != 1) return EXIT_FAILURE; break;
            case 'f': if(sscanf(optarg, "%d"  , &p->log_filedes ) != 1) return EXIT_FAILURE; break;
            case 'p': if(sscanf(optarg, "%d"  , &p->pipe_filedes) != 1) return EXIT_FAILURE; break;
            default: opterr = 1; optind = 1; return EXIT_FAILURE;
        }
    }
    opterr = 1;

    p->filesc = argc - optind;
    p->files  = malloc(p->filesc*sizeof(char *));
    if(p->files == NULL) return EXIT_FAILURE;
    for(int i = 0; i < p->filesc; ++i){
        const char *s = argv[optind+i];
        char *str = malloc((1+strlen(s)*sizeof(char)));
        if(str == NULL){
            for(int j = 0; j < i; ++j) free(p->files[i]);
            free(p->files);
            return EXIT_FAILURE;
        }
        strcpy(str, s);
        p->files[i] = str;
    }
    optind = 1;

    if(p->filesc == 0){
        p->filesc = 1;
        p->files  = malloc(p->filesc*sizeof(char *));
        if(p->files == NULL) return EXIT_FAILURE;
        const char *s = ".";
        char *str = malloc(2*sizeof(char));
        if(str == NULL){
            free(p->files);
            return EXIT_FAILURE;
        }
        strcpy(str, s);
        p->files[0] = str;
    }
    return EXIT_SUCCESS;
}

int simpledu_args_dtor(simpledu_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    for(int i = 0; i < p->filesc; ++i){
        free(p->files[i]);
        p->files[i] = NULL;
    }
    free(p->files);
    p->files = NULL;
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

#define TOARGV_MIN_ARGC 8
#define TOARGV_BUF_SIZE 4096

int simpledu_args_toargv(const simpledu_args_t *p, char ***pargv){
    if(p == NULL){ errno = EINVAL; return EXIT_FAILURE; }

    char **argv = malloc((TOARGV_MIN_ARGC+p->filesc+1)*sizeof(char *));
    
    char buf[TOARGV_BUF_SIZE];
    int i = 0;
    {                     strcpy(buf, "./simpledu"     ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //Program name
    if(p->all          ){ strcpy(buf, "-a"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-a, --all
    { sprintf(buf, "--blocksize=%llu"  , p->block_size ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-B, --blocksize
    if(p->count_links  ){ strcpy(buf, "-l"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-l, --count-links
    if(p->dereference  ){ strcpy(buf, "-L"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-L, --dereference
    if(p->separate_dirs){ strcpy(buf, "-S"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-S, --separate-dirs
    { sprintf(buf, "--max-depth=%hu"  , p->max_depth   ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-d, --max-depth
    { sprintf(buf, "--log-filedes=%d" , p->log_filedes ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //log_filedes
    { sprintf(buf, "--pipe-filedes=%d", p->pipe_filedes); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //log_filedes

    for(int j = 0; j < p->filesc; ++i, ++j){
        argv[i] = malloc((strlen(p->files[j])+1)*sizeof(char));
        strcpy(argv[i], p->files[j]);
    }

    argv[i++] = NULL;

    *pargv = argv;
    return EXIT_SUCCESS;
}
