#include "simpledu_args.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

static const simpledu_args_t simpledu_args_default = 
{ 
.all = false,
.apparent_size = false,
.block_size = 1024,
.dereference = false, 
.count_links = false, 
.separate_dirs = false, 
.max_depth = PATH_MAX/2, 
.log_filedes = -1, 
.pipe_filedes = -1,
.start_time = -1,
.children_process_group = 0,
.filesc = 0, 
.files = NULL
};

static const char optstring[] = "abLlSB:d:f:p:s:";
static int apparent_size = false;
static int children_process_group = false;
static const struct option longopts[] = {
    {"all"          , 0, NULL, 'a'},
    {"bytes"        , 0, NULL, 'b'},
    {"dereference"  , 0, NULL, 'L'},
    {"count-links"  , 0, NULL, 'l'},
    {"separate-dirs", 0, NULL, 'S'},
    {"apparent-size", 0, &apparent_size, 1},
    {"block-size"   , 1, NULL, 'B'},
    {"max-depth"    , 1, NULL, 'd'},
    {"log-filedes"  , 1, NULL, 'f'},
    {"pipe-filedes" , 1, NULL, 'p'},
    {"start-time"   , 1, NULL, 's'},
    {"children-process-group", 1, &children_process_group, 1},
    {0,0,0,0}
};

int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]){
    int ret = EXIT_SUCCESS;

    if(p == NULL){ errno = EINVAL; return EXIT_FAILURE; }
    *p = simpledu_args_default;

    if(argc == 0 || argv == NULL) return EXIT_SUCCESS;

    opterr = 0;
    optind = 1;
    int opt = 0; int longindex;
    while((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1){
        switch(opt){
            case 'a': p->all           = true; break;
            case 'b': p->block_size    =    1; p->apparent_size = true; break;
            case 'L': p->dereference   = true; break;
            case 'l': p->count_links   = true; break;
            case 'S': p->separate_dirs = true; break;
            case 'B': if(sscanf(optarg, "%llu" , &p->block_size  ) != 1) return EXIT_FAILURE; break;
            case 'd': if(sscanf(optarg, "%hd"  , &p->max_depth   ) != 1) return EXIT_FAILURE; break;
            case 'f': if(sscanf(optarg, "%d"   , &p->log_filedes ) != 1) return EXIT_FAILURE; break;
            case 'p': if(sscanf(optarg, "%d"   , &p->pipe_filedes) != 1) return EXIT_FAILURE; break;
            case 's': if(sscanf(optarg, "%lldd", &p->start_time  ) != 1) return EXIT_FAILURE; break;
            case 0:
                if(apparent_size){
                    p->apparent_size = true;
                    apparent_size = false;
                } else if(children_process_group){
                    if(sscanf(optarg, "%d" , &p->children_process_group) != 1) return EXIT_FAILURE;
                }
                break;
            case '?':
                fprintf(stderr, "du: invalid option -- '%c'\n", optopt);
                ret = EXIT_FAILURE;
                break;
            default:
                opterr = 1; optind = 1; return EXIT_FAILURE;
        }
    }
    opterr = 1;

    p->filesc = argc - optind;
    if(p->filesc != 0) {
        p->files  = malloc(p->filesc*sizeof(char *));
        if(p->files == NULL) return EXIT_FAILURE;
        for(size_t i = 0; i < p->filesc; ++i){
            const char *s = argv[optind+i];
            char *str = malloc((1+strlen(s)*sizeof(char)));
            if(str == NULL){
                for(size_t j = 0; j < i; ++j) free(p->files[i]);
                free(p->files);
                return EXIT_FAILURE;
            }
            strcpy(str, s);
            p->files[i] = str;
        }
        optind = 1;
    } else {
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

    if(ret) fprintf(stderr, "Try 'du --help' for more information.\n");
    return ret;
}

int simpledu_args_dtor(simpledu_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    for(size_t i = 0; i < p->filesc; ++i){
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
        p1->log_filedes     == p2->log_filedes      && 
        p1->pipe_filedes    == p2->pipe_filedes     && 
        p1->start_time      == p2->start_time       && 
        p1->children_process_group == p2->children_process_group && 
        p1->filesc          == p2->filesc
    )) return false;
    for(size_t i = 0; i < p1->filesc; ++i){
        if(strcmp(p1->files[i], p2->files[i]) != 0) return false;
    }
    return true;
}

int simpledu_args_copy(simpledu_args_t *p1, const simpledu_args_t *p2){
    p1->all             = p2->all;
    p1->apparent_size   = p2->apparent_size;
    p1->block_size      = p2->block_size;
    p1->count_links     = p2->count_links;
    p1->dereference     = p2->dereference;
    p1->separate_dirs   = p2->separate_dirs;
    p1->max_depth       = p2->max_depth;
    p1->log_filedes     = p2->log_filedes;
    p1->pipe_filedes    = p2->pipe_filedes;
    p1->start_time      = p2->start_time;
    p1->children_process_group = p2->children_process_group;
    p1->filesc          = p2->filesc;
    p1->files = malloc(p1->filesc*sizeof(char*));
    for(size_t i = 0; i < p1->filesc; ++i){
        p1->files[i] = malloc((strlen(p2->files[i])+1)*sizeof(char));
        if(p1->files[i] == NULL) return EXIT_FAILURE;
        strcpy(p1->files[i], p2->files[i]);
    }
    return EXIT_SUCCESS;
}

int simpledu_args_set_files(simpledu_args_t *p, int num,...){
    va_list valist;
    va_start(valist, num);
    for(size_t i = 0; i < p->filesc; ++i){
        free(p->files[i]);
        p->files[i] = NULL;
    }
    free(p->files); p->files = NULL;
    p->filesc = num;
    p->files = malloc(p->filesc*sizeof(char*));
    for(size_t i = 0; i < p->filesc; ++i){
        char *s = va_arg(valist, char*);
        p->files[i] = malloc((strlen(s)+1)*sizeof(char));
        if(p->files[i] == NULL) return EXIT_FAILURE;
        strcpy(p->files[i], s);
    }
    return EXIT_SUCCESS;
}

#define TOARGV_MIN_ARGC 32
#define TOARGV_BUF_SIZE 4096

int simpledu_args_toargv(const simpledu_args_t *p, char ***pargv){
    if(p == NULL){ errno = EINVAL; return EXIT_FAILURE; }

    char **argv = malloc((TOARGV_MIN_ARGC+p->filesc+1)*sizeof(char *));
    
    char buf[TOARGV_BUF_SIZE];
    int i = 0;
    {                     strcpy(buf, "./simpledu"     ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //Program name
    if(p->all          ){ strcpy(buf, "-a"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-a, --all
    if(p->apparent_size){ strcpy(buf, "--apparent-size"); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-a, --all
    { sprintf(buf, "--block-size=%llu"  , p->block_size); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-B, --blocksize
    if(p->count_links  ){ strcpy(buf, "-l"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-l, --count-links
    if(p->dereference  ){ strcpy(buf, "-L"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-L, --dereference
    if(p->separate_dirs){ strcpy(buf, "-S"             ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-S, --separate-dirs
    { sprintf(buf, "--max-depth=%hu"  , p->max_depth   ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //-d, --max-depth
    { sprintf(buf, "--log-filedes=%d" , p->log_filedes ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //log_filedes
    { sprintf(buf, "--pipe-filedes=%d", p->pipe_filedes); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //pipe_filedes
    { sprintf(buf, "--start-time=%lld", p->start_time  ); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //pipe_filedes
    { sprintf(buf, "--children-process-group=%d", p->children_process_group); strcpy(argv[i++] = malloc((strlen(buf)+1)*sizeof(char)), buf); } //pipe_filedes
    
    for(size_t j = 0; j < p->filesc; ++i, ++j){
        argv[i] = malloc((strlen(p->files[j])+1)*sizeof(char));
        strcpy(argv[i], p->files[j]);
    }

    argv[i++] = NULL;

    *pargv = argv;
    return EXIT_SUCCESS;
}