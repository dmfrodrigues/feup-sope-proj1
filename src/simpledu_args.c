#include "simpledu_args.h"

#include <stdlib.h>

const simpledu_args_t simpledu_args_default = {false, false, 1024, false, false, false, PATH_MAX/2};

int simpledu_args_ctor(simpledu_args_t *p, int argc, char *argv[]){
    *p = simpledu_args_default;
    return 0;
}