#include "simpledu_envp.h"

#include <stdlib.h>
#include <string.h>

static const simpledu_envp_t simpledu_envp_default = {NULL};

int simpledu_envp_ctor(simpledu_envp_t *p, char *envp[]){
    *p = simpledu_envp_default;
    char *LOG_FILENAME = getenv("LOG_FILENAME");
    if(LOG_FILENAME != NULL){
        p->LOG_FILENAME = malloc((strlen(LOG_FILENAME)+1)*sizeof(char));
        strcpy(p->LOG_FILENAME, LOG_FILENAME);
    }
    return EXIT_SUCCESS;
}

int simpledu_envp_dtor(simpledu_envp_t *p){
    free(p->LOG_FILENAME);
    return EXIT_SUCCESS;
}
