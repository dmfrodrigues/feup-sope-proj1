#include <stdlib.h>
#include <stdio.h>

#include "simpledu_args.h"
#include "simpledu_envp.h"
#include "simpledu_time.h"
#include "simpledu_log.h"

int main(int argc, char *argv[], char *envp[]){
    if(simpledu_starttime()) return EXIT_FAILURE;

    simpledu_args_t p;
    simpledu_args_ctor(&p, argc, argv);
    simpledu_args_dtor(&p);

    simpledu_envp_t q;
    simpledu_envp_ctor(&q, envp);
    simpledu_envp_dtor(&q);

    simpledu_exit(0);
}
