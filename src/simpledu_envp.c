#include "simpledu_envp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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

int simpledu_set_exec_path(char *envp[]){
    char pwd[4096];
    for (char **env = envp; *env != 0; env++){
       char *thisEnv = *env;

        if (strstr(thisEnv, "OLD") != NULL){
            continue;
        }

       if (strstr(thisEnv, "PWD") != NULL){
           strcpy(pwd, thisEnv + 4);
       }
    }

    if (getenv("EXEC_PATH") == NULL){
        strcat(pwd, "/simpledu");
        if (setenv("EXEC_PATH", pwd, 1) == 0) {
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
