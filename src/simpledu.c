#include <stdlib.h>
#include <stdio.h>

#include "simpledu_args.h"
#include "simpledu_envp.h"
#include "simpledu_time.h"

int main(int argc, char *argv[], char *envp[]){
    if(simpledu_starttime()) return EXIT_FAILURE;

    simpledu_args_t p;
    simpledu_args_ctor(&p, argc, argv);
    return 0;
}
