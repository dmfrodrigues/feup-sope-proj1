#include "simpledu_envp.h"

#include <stdlib.h>

static const simpledu_envp_t simpledu_envp_default = {NULL};

int simpledu_envp_ctor(simpledu_envp_t *p, char *envp[]){
    *p = simpledu_envp_default;
    p->LOG_FILENAME = getenv("LOG_FILENAME");
    return EXIT_SUCCESS;
}

int simpledu_envp_dtor(simpledu_envp_t *p){
    free(p->LOG_FILENAME);
    return EXIT_SUCCESS;
}